#include "CommonFollowerUtil.h"

std::vector<RE::TESObjectREFR*> FollowerCollection::_objectListMale = std::vector<RE::TESObjectREFR*>();
std::vector<RE::TESObjectREFR*> FollowerCollection::_objectListFemale = std::vector<RE::TESObjectREFR*>();
std::unordered_map<std::string, int> FollowerCollection::_dlcMap = std::unordered_map<std::string, int>();
bool FollowerCollection::_enableFollowerFunction = true;
uint32_t FollowerCollection::_upperlimit = 0;

std::string TESReader::readTESType(FILE* hFile, uint32_t pos) {
    char cstr[5];
    fseek(hFile, pos, 0);
    fread(cstr, sizeof(char), sizeof(cstr) - 1, hFile);
    cstr[4] = '\0';
    return cstr;
}

std::string TESReader::readTESString(FILE* hFile, uint32_t pos, uint32_t length) {
    char cstr[256];
    fseek(hFile, pos, 0);
    fread(cstr, sizeof(char), length, hFile);
    return cstr;
}

uint32_t TESReader::readUInt32(FILE* hFile, uint32_t pos) {
    uint32_t value;
    fseek(hFile, pos, 0);
    fread(&value, sizeof(value), 1, hFile);
    return value;
}

uint32_t TESReader::readUInt16(FILE* hFile, uint32_t pos) {
    uint16_t value;
    fseek(hFile, pos, 0);
    fread(&value, sizeof(value), 1, hFile);
    return value;
}

TESFileReader::TESFileReader() {}

TESFileReader::TESFileReader(uint32_t modNum) {
    _file = NULL;
    _modIndex = modNum;
    _pos = 0;
    _hasRefs = false;
    _hasCells = false;
    _isError = false;
    _hasBody = true;
    _errorType = "";
    _upperLimit = 0;
    std::vector<pair> _pairs = std::vector<pair>();
    std::vector<std::string> _masters = std::vector<std::string>();
    _masterNum = 0;
    _dhnd = RE::TESDataHandler::GetSingleton();

    _setFilePath();
}

TESFileReader::~TESFileReader() {}

std::string TESFileReader::readTESType() {
    if (_pos >= _eofPos) {
        _isError = true;
        _errorType = "File read pointer error. (Unexpected ESP file format?)";
        return "";
    }
    return TESReader::readTESType(_file, _pos);
}

std::string TESFileReader::readTESString(uint32_t length) {
    if (_pos >= _eofPos) {
        _isError = true;
        _errorType = "File read pointer error. (Unexpected ESP file format?)";
        return "";
    }
    return TESReader::readTESString(_file, _pos, length);
}

uint32_t TESFileReader::readUInt32() {
    if (_pos >= _eofPos) {
        _isError = true;
        _errorType = "File read pointer error. (Unexpected ESP file format?)";
        return NULL;
    }
    return TESReader::readUInt32(_file, _pos);
}

uint32_t TESFileReader::readUInt16() {
    if (_pos >= _eofPos) {
        return NULL;
    }
    return TESReader::readUInt16(_file, _pos);
}



void TESFileReader::_setFilePath() {
    //_filePath = GetRuntimeDirectory() + "data\\" + _dhnd->modList.loadedMods[_modIndex]->name;
    _filePath = Utility::GetCurrentExecutableDirectory() + "\\data\\" + _dhnd->LookupLoadedModByIndex(_modIndex)->fileName;

    //_modName = _dhnd->modList.loadedMods[_modIndex]->name;
    _modName = _dhnd->LookupLoadedModByIndex(_modIndex)->fileName;
    std::string ends = ".esp";
    if (TESFileReader::_ends_with(_modName, ends)) {
        _modType = "esp";
    } else {
        _modType = "other";
    }
}

bool TESFileReader::isEsp() {
    if (_modType == "esp") {
        return true;
    } else {
        return false;
    }
}

void TESFileReader::setUpperLimit(uint32_t upperLimit) { _upperLimit = upperLimit; }

void TESFileReader::scanTESFileByModIndex() {
    _openTESFile();
    _getTESHeader();
    if (_hasBody) {
        _skipTESHeader();
        _skipToCellGroup();
        _collectACHRFormIDs();
    }
    _closeTESFile();
}

bool TESFileReader::_ends_with(std::string str, std::string end) {
    if (str.size() >= end.size() && str.find(end, str.size() - end.size()) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

void TESFileReader::_openTESFile() {
    fopen_s(&_file, _filePath.c_str(), "rb");
    if (_file == NULL || _file == INVALID_HANDLE_VALUE) {
        logs::info("can't open file {}", _filePath.c_str());
        fclose(_file);
        _isError = true;
        _errorType = "can't open file";
        return;
    } else {
        fpos_t fsize = 0;

        fseek(_file, 0, SEEK_END);
        fgetpos(_file, &fsize);
        _eofPos = fsize;
        if (_upperLimit == 0 || _upperLimit > fsize) {
            return;
        } else {
            fclose(_file);
            _isError = true;
            _errorType = "skip for file size upper limit";
            logs::info("TESFileReader::{}'s file size({}) exceeds the upper limit({})", _filePath.c_str(), fsize, _upperLimit);
            return;
        }
        // logs::info("TESFileReader::%s: %d bytes", _filePath.c_str(), _eofPos);
    }
}

void TESFileReader::_closeTESFile() { fclose(_file); }

void TESFileReader::_getTESHeader() {
    if (_isError) {
        //		logs::info("TESFileReader::_getTESHeader:\tskip");
        return;
    }
    int backup = _pos;

    _pos = 24 + 4;  // skip TES4 record and "HEDR" label.
    fseek(_file, _pos, 0);

    uint32_t dataSize;
    dataSize = readUInt16();
    _pos += HALFSIZE + dataSize;

    // read CNAM if needed
    std::string label;
    label = readTESType();
    if (label == "CNAM") {
        _pos += UNITSIZE;
        dataSize = readUInt16();
        _pos += HALFSIZE + dataSize;
        label = readTESType();
    }
    if (label == "SNAM") {
        _pos += UNITSIZE;
        dataSize = readUInt16();
        _pos += HALFSIZE + dataSize;
        label = readTESType();
    }
    int index = 0;
    std::string modName;
    while (label == "MAST" && _pos < _eofPos) {
        _pos += UNITSIZE;
        dataSize = readUInt16();
        _pos += HALFSIZE;
        modName = readTESString(dataSize);
        //		logs::info("MAST: %d: %s", index, modName.c_str());
        _masters.push_back(modName);
        _pos += dataSize;
        _pos += UNITSIZE * 3 + HALFSIZE;
        index++;
        if (_pos < _eofPos) {
            label = readTESType();
        }
    }
    //	logs::info("TESFileReader:: current_pos: %d, eofPos: %d", _pos, _eofPos);
    if (_pos >= _eofPos) {
        _hasBody = false;
    }

    _masterNum = index;
    _pos = backup;
}

void TESFileReader::_skipTESHeader() {
    if (_isError) {
        //		logs::info("TESFileReader::_skipTESHeader:\tskip");
        return;
    }

    _pos = 4;
    fseek(_file, _pos, 0);

    uint32_t dataSize = readUInt32();
    _pos = HEADERSIZE + dataSize;
    fseek(_file, _pos, 0);

    std::string type = readTESType();
    if (type == "GRUP") {
        //		logs::info("skipTESHeader: ok, current pos is %s", type.c_str());
    } else {
        //		logs::info("TESFileReader::_skipTESHeader:\can't get first GRUP");
        _isError = true;
        _errorType = "can't get first GRUP.";
        return;
    }
}

void TESFileReader::_skipToCellGroup() {
    if (_isError) {
        //		logs::info("TESFileReader::_skipToCellGroup:\tskip");
        return;
    }

    TESGroup tg;
    tg.label = "";

    bool isSuccess = false;
    while (_pos < _eofPos && tg.label != "CELL") {
        tg = _fetchNextTESGroup();
        if (tg.label == "CELL") {
            isSuccess = true;
            _cellEndPos = _pos + tg.dataSize;
        } else {
            _pos += tg.dataSize;
        }
    }

    if (isSuccess) {
        _hasCells = true;
    } else {
        //		logs::info("TESFileReader::_skipToCellGroup:\tthis file does not have CELL GRUP.");
        _hasCells = false;
        _isError = true;
        _errorType = "this file does not have CELL GRUP.";
    }
}

TESGroup TESFileReader::_fetchNextTESGroup() {
    TESGroup tg;
    uint32_t posBackup = _pos;
    tg.type = readTESType();
    _pos += UNITSIZE;
    tg.dataSize = readUInt32();
    _pos += UNITSIZE;
    tg.label = readTESType();
    _pos = posBackup;

    return tg;
}

TESRecord TESFileReader::_fetchNextTESRecord() {
    TESRecord tr;
    uint32_t posBackup = _pos;

    tr.type = readTESType();
    _pos += UNITSIZE;
    tr.dataSize = readUInt32();
    _pos += UNITSIZE;
    tr.flags = readUInt32();
    _pos += UNITSIZE;
    tr.formID = readUInt32();
    _pos += UNITSIZE;

    _pos = posBackup;

    return tr;
}

TESFieldUnknown TESFileReader::_fetchNextTESFieldUnknown() {
    TESFieldUnknown tf;
    uint32_t posBackup = _pos;

    tf.type = readTESType();
    _pos += UNITSIZE;
    tf.dataSize = readUInt16();
    _pos += HALFSIZE;

    _pos = posBackup;

    return tf;
}

TESFieldTarget TESFileReader::_fetchNextTESFieldTarget() {
    TESFieldTarget tf;
    uint32_t posBackup = _pos;

    tf.type = readTESType();
    _pos += UNITSIZE;
    tf.dataSize = readUInt16();
    _pos += HALFSIZE;
    tf.formID = readUInt32();
    _pos += UNITSIZE;

    _pos = posBackup;

    return tf;
}

void TESFileReader::_collectACHRFormIDs() {
    if (_isError) {
        //		logs::info("TESFileReader::_collectACHRFormIDs:\tskip");
        return;
    }

    TESGroup tg = _fetchNextTESGroup();

    while (_pos < _cellEndPos) {
        if (tg.type == "GRUP") {
            _pos += GRUPSIZE;
            tg = _fetchNextTESGroup();
        } else {
            if (tg.type != "ACHR") {
                _pos += HEADERSIZE + tg.dataSize;
            } else {
                TESRecord tr = _fetchNextTESRecord();
                uint32_t nextPos = _pos + HEADERSIZE + tr.dataSize;
                _hasRefs = true;

                _pos += HEADERSIZE;
                TESFieldUnknown tf = _fetchNextTESFieldUnknown();
                bool isSuccess = false;
                uint32_t abformID = 0;
                while (_pos < nextPos && isSuccess == false) {
                    if (tf.type == "NAME") {
                        TESFieldTarget tfg = _fetchNextTESFieldTarget();
                        abformID = tfg.formID;
                        isSuccess = true;
                    } else {
                        _pos += UNITSIZE + HALFSIZE + tf.dataSize;
                        tf = _fetchNextTESFieldUnknown();
                    }
                }
                uint32_t objID = _modifyModIndexMask(tr.formID);
                uint32_t baseID = _modifyModIndexMask(abformID);
                _pushItem(objID, baseID);
                _pos = nextPos;
                //			logs::info("TESFileReader::_collectACHRFormIDs:\t%08X(%08X)\t%08X(%08X)\t%s", objID, tr.formID,
                //baseID, abformID, _modName.c_str());
            }
            tg = _fetchNextTESGroup();
        }
    }
    if (_pos != _cellEndPos) {
        _isError = true;
        _errorType = "this file's read-pos does not match cellEndPos.";
        //		logs::info("TESFileReader::_collectACHRFormIDs:\tUnmatch pos(%d) & cellEndPos(%d)", _pos, _cellEndPos);
    }
    if (_hasRefs) {
        //		logs::info("TESFileReader::_collectACHRFormIDs:\t%s has refs", _filePath.c_str());
    }
}

/*
class LoadedModFinder
{
    const char * m_stringToFind;

public:
    LoadedModFinder(const char * str) : m_stringToFind(str) { }

    bool Accept(ModInfo* modInfo)
    {
        return _stricmp(modInfo->name, m_stringToFind) == 0;
    }
};
*/

uint32_t TESFileReader::_modifyModIndexMask(uint32_t formID) {
    uint32_t mask = formID >> 24;
    uint32_t newMask;
    uint32_t modIndex;

    //	logs::info("\t\toriginal formID=%08X, mask = %02X", formID, mask);

    if (mask == _masterNum) {
        //		logs::info("\t\tthis formID(%08X) has mask(%02X), this mask should be replaced to original modIndex %02X",
        //formID, mask, _modIndex);
        newMask = _modIndex << 24;
    } else if (mask < _masterNum) {
        //		modIndex = _dhnd->modList.modInfoList.GetIndexOf(LoadedModFinder(_masters[mask].c_str()));
        //modIndex = _dhnd->LookupModByName(_masters[mask].c_str())->modIndex;
        modIndex = _dhnd->GetLoadedModIndex(_masters[mask].c_str()).value();
         //		logs::info("\t\tthis formID(%08X) has mask(%02X), masters[%d]=%s, this mask should be replaced to
        //master's
        //modIndex %02d", formID, mask, mask, _masters[mask].c_str(), modIndex);
        newMask = modIndex << 24;
    } else {
        //		logs::info("\t\terror: this formID(%08X) has mask(%02X), but masterNum=%d", formID, mask, _masterNum);
        return 0x0;
    }

    uint32_t newFormID = formID & 0x00FFFFFF | newMask;

    //	logs::info("\t\toriginal formID=%08X, oldMask = %02X, newMask = %02X, new formID=%08X", formID, mask, newMask >>
    //24, newFormID);

    return newFormID;
}

void TESFileReader::_pushItem(uint32_t objID, uint32_t baseID) {
    pair item;
    item.objID = objID;
    item.baseID = baseID;
    item.modName = _modName;
    _pairs.push_back(item);
}

std::vector<pair> TESFileReader::getRefs() { return _pairs; }

bool TESFileReader::hasRefs() { return _hasRefs; }

bool TESFileReader::hasCells() { return _hasCells; }

std::string TESFileReader::getModName() { return _modName; }

bool TESFileReader::hasError() { return _isError; }

std::string TESFileReader::getErrorType() { return _errorType; }

FollowerCollection::FollowerCollection() {}

FollowerCollection::~FollowerCollection() {}

std::string FollowerCollection::_getIniFilePath() {
    return Utility::GetCurrentExecutableDirectory() + "\\Data\\SKSE\\plugins\\EstrusForSkyrimNG.ini";
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> res;
    size_t current = 0, found;
    while ((found = str.find_first_of(delim, current)) != std::string::npos) {
        res.push_back(std::string(str, current, found - current));
        current = found + 1;
    }
    res.push_back(std::string(str, current, str.size() - current));
    return res;
}

bool FollowerCollection::_isDlc(std::string modName) { return (modName == "Skyrim.esm" || _dlcMap[modName] != 0); }

void FollowerCollection::_setupDLCsMap() {
    RE::TESDataHandler* dhnd = RE::TESDataHandler::GetSingleton();
    if (dhnd->LookupModByName("Skyrim.esm") != NULL) {
        //		_dlcMap["Skyrim.esm"] = _dhnd->modList.modInfoList.GetIndexOf(LoadedModFinder("Skyrim.esm"));
        //		_dlcMap["Skyrim.esm"] = dhnd->GetLoadedModIndex("Skyrim.esm");
        //_dlcMap["Skyrim.esm"] = dhnd->LookupModByName("Skyrim.esm")->modIndex;
        _dlcMap["Skyrim.esm"] = dhnd->GetLoadedModIndex("Skyrim.esm").value();
    }
    if (dhnd->LookupModByName("Dawnguard.esm") != NULL) {
        //		_dlcMap["Skyrim.esm"] = _dhnd->modList.modInfoList.GetIndexOf(LoadedModFinder("Dawnguard.esm"));
        //_dlcMap["Dawnguard.esm"] = dhnd->LookupModByName("Dawnguard.esm")->modIndex;
        _dlcMap["Dawnguard.esm"] = dhnd->GetLoadedModIndex("Dawnguard.esm").value();
    }
    if (dhnd->LookupModByName("HearthFires.esm") != NULL) {
        //		_dlcMap["Skyrim.esm"] = _dhnd->modList.modInfoList.GetIndexOf(LoadedModFinder("HearthFires.esm"));
        //_dlcMap["HearthFires.esm"] = dhnd->LookupModByName("HearthFires.esm")->modIndex;
        _dlcMap["HearthFires.esm"] = dhnd->GetLoadedModIndex("HearthFires.esm").value();
    }
    if (dhnd->LookupModByName("Dragonborn.esm") != NULL) {
        //		_dlcMap["Skyrim.esm"] = _dhnd->modList.modInfoList.GetIndexOf(LoadedModFinder("Dragonborn.esm"));
        //_dlcMap["Dragonborn.esm"] = dhnd->LookupModByName("Dragonborn.esm")->modIndex;
        _dlcMap["Dragonborn.esm"] = dhnd->GetLoadedModIndex("Dragonborn.esm").value();
    }
}

iniConf FollowerCollection::_str2Conf(std::string str) {
    std::vector<std::string> items = split(str, '\t');
    iniConf item;
    item.key = items[0];
    item.value = items[1];
    return item;
}

iniLine FollowerCollection::_str2item(std::string str) {
    std::vector<std::string> items = split(str, '\t');
    iniLine item;
    item.modName = items[0];
    item.formID = 0;
    item.objectID = 0;
    item.isFemale = (items[3] == "Female");
    item.name = items[4];

    if (item.modName != "Skyrim.esm" && _dlcMap[items[0]] == 0) {
        return item;
    }

    uint32_t upper2digit = _dlcMap[items[0]] << 24;
    item.objectID = std::strtoul(items[1].c_str(), NULL, 16);
    item.objectID = item.objectID | upper2digit;
    item.formID = std::strtoul(items[2].c_str(), NULL, 16);
    item.formID = item.formID | upper2digit;

    return item;
}

void FollowerCollection::_readIniFile() {
    _setupDLCsMap();
    std::string iniFile = _getIniFilePath();
    logs::info("iniFilePath = {}", iniFile.c_str());
    std::ifstream ifs(iniFile);
    if (ifs.fail()) {
        logs::info("error, can't open {}", iniFile.c_str());
        return;
    }
    /*
    ini image
    Skyrim.esm	000198B0	0001325F	Male	Ahtar
    ...
    */
    std::smatch tagMatch;
    std::regex reComment("^\\s*#.*$");
    std::regex reEmpty("^\\s*$");
    std::regex reConf("^\\s*(\\S+)\\t(\\w+)\\t(\\w+)\\t(\\w+)\\t(.*)$");
    std::regex reConfParam("^\\s*(\\S+)\\t(\\w+)$");

    std::string str;
    std::string currentTag = "";
    while (getline(ifs, str)) {
        if (std::regex_match(str, reComment)) {
            //	nop
        } else if (std::regex_match(str, reEmpty)) {
            //	nop
        } else if (std::regex_match(str, reConf)) {
            iniLine item = _str2item(str);

            //TESNPC* npc = (TESNPC*)LookupFormByID(item.formID);
            RE::TESNPC* npc = RE::TESForm::LookupByID<RE::TESNPC>(item.formID);
            //RE::TESObjectREFR* objRef = (RE::TESObjectREFR*)LookupFormByID(item.objectID);
            RE::TESObjectREFR* objRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(item.objectID);
            if (objRef && npc) {
                logs::info("FollowerCollection::readIniFile:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\texist", item.objectID,
                         item.formID, item.modName.c_str(), item.name.c_str());
                //if (CALL_MEMBER_FN(npc, GetSex)()) {
                if (npc->GetSex() == RE::SEXES::kFemale) {
                        _objectListFemale.push_back(objRef);
                    //logs::info("%s is female", npcName.c_str());
                } else {
                    _objectListMale.push_back(objRef);
                    //logs::info("%s is male", npcName.c_str());
                }
            } else {
                logs::info("FollowerCollection::readIniFile:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\tNOT EXIST", item.objectID,
                         item.formID, item.modName.c_str(), item.name.c_str());
            }
        } else if (std::regex_match(str, reConfParam)) {
            iniConf conf = _str2Conf(str);
            logs::info("FollowerCollection::readIniFile:\t{}\t{}", conf.key.c_str(), conf.value.c_str());
            _setconf(conf);
        } else {
            logs::info("ERROR: invalid line: {}", str.c_str());
        }
    }
}

void FollowerCollection::_setconf(iniConf conf) {
    if (conf.key == "FILE_UPPER_LIMIT_SIZE") {
        _upperlimit = std::stoi(conf.value);
    } else if (conf.key == "GET_FOLLOWER_FROM_OTHER_MOD") {
        if (conf.value == "OFF") {
            _enableFollowerFunction = false;
        }
    }
}

void FollowerCollection::collectAll() {
    RE::TESDataHandler* dhnd = RE::TESDataHandler::GetSingleton();

    _readIniFile();
    if (!_enableFollowerFunction) {
        logs::info("FollowerCollection::collectionAll:\tGET_FOLLOWER_FROM_OTHER_MOD OFF");
        return;
    }
    uint32_t modCount;
    //	modCount = dhnd->modList.loadedModCount;
    //modCount = dhnd->modList.loadedMods.count;
    modCount = dhnd->GetLoadedModCount();
    for (int i = 0; i < modCount; i++) {
        class TESFileReader trf = TESFileReader(i);
        trf.setUpperLimit(_upperlimit);
        //		_DMESSAGE("debug: start modName=%s", trf.getModName().c_str());
        //		if(trf.isEsp()){
        if (!_isDlc(trf.getModName())) {
            trf.scanTESFileByModIndex();
            if (trf.hasError()) {
                //				logs::info("debug: An error occurred in reading this MOD. Skip reading this MOD.\n\t%s:\t%s",
                //trf.getModName().c_str(), trf.getErrorType().c_str());
            } else {
                //				logs::info("Reading of this MOD ended normally..\n\t%s", trf.getModName().c_str());
            }
            std::vector<pair> pairs = trf.getRefs();
            std::vector<pair>::iterator it;
            for (it = pairs.begin(); it != pairs.end(); it++) {
                //				logs::info("\tdebug: baseID=%08X, objID=%08X", it->baseID, it->objID);
                //TESNPC* npc = (TESNPC*)LookupFormByID(it->baseID);
                //RE::TESObjectREFR* objRef = (RE::TESObjectREFR*)LookupFormByID(it->objID);
                RE::TESNPC* npc = RE::TESForm::LookupByID<RE::TESNPC>(it->baseID);
                RE::TESObjectREFR* objRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(it->objID);
                //				logs::info("\tdebug: lookup done");
                //if (npc && npc->kTypeID == 43) {
                if (npc && npc->GetFormType() == RE::FormType::NPC) {
                        //					logs::info("\tdebug: npc->kTypeID=%d", npc->kTypeID);
                    // std::string npcName = npc->fullName.GetName();
                    std::string npcName = npc->GetFullName();
                    //int numFactions = npc->RE::ActorData.factions.count;
                    int numFactions = npc->factions.size();
                    if (numFactions > 0) {
                        for (int j = 0; j < numFactions; j++) {
                            RE::TESFaction* pFaction;
                            // pFaction = npc->RE::ActorData.factions.arr.entries[j].faction;
                            pFaction = npc->factions[j].faction;

                            if (pFaction && pFaction->formID == 378957) {
                                if (objRef) {
                                    logs::info(
                                        "FollowerCollection::collectionAll:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\texist",
                                        it->objID, it->baseID, it->modName.c_str(), npcName.c_str());
                                    // if (CALL_MEMBER_FN(npc, GetSex)()) {
                                    if (npc->GetSex() == RE::SEXES::kFemale) {
                                        _objectListFemale.push_back(objRef);
                                        //logs::info("%s is female", npcName.c_str());
                                    } else {
                                        _objectListMale.push_back(objRef);
                                        //logs::info("%s is male", npcName.c_str());
                                    }
                                } else {
                                    logs::info(
                                        "FollowerCollection::collectionAll:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\tNOT EXIST",
                                        it->objID, it->baseID, it->modName.c_str(), npcName.c_str());
                                }
                            }
                        }
                    }
                }
            }
        } else {
            logs::info("FollowerCollection::collectionAll():\t{} is Default DLC file",
                      //dhnd->modList.loadedMods[i]->name);
                      dhnd->LookupLoadedModByIndex(i)->fileName);
        }
        //		_DMESSAGE("debug: finish modName=%s", trf.getModName().c_str());
    }
}

uint32_t FollowerCollection::getFollowersCountMale() { return _objectListMale.size(); }

RE::TESObjectREFR* FollowerCollection::getFollowerObjectMale(uint32_t num) {
    logs::info("FollowerCollection::getFollowerObjectMale\tnum={}\tRE::Actor={:08X}", num, _objectListMale[num]->formID);
    return _objectListMale[num];
}

RE::TESObjectREFR* FollowerCollection::getRandomFollowerObjectMale() {
    uint32_t rand;
    std::random_device rnd;
    rand = rnd() % getFollowersCountMale();
    RE::TESObjectREFR* actor = _objectListMale[rand];
    logs::info("FollowerCollection::getRandomFollowerObjectMale\trand={}\tRE::Actor={:08X}", rand, actor->formID);
    return _objectListMale[rand];
}

uint32_t FollowerCollection::getFollowersCountFemale() { return _objectListFemale.size(); }

RE::TESObjectREFR* FollowerCollection::getFollowerObjectFemale(uint32_t num) {
    logs::info("FollowerCollection::getRandomFollowerObjectFemale\tnum={}\tRE::Actor={:08X}", num,
             _objectListFemale[num]->formID);
    return _objectListFemale[num];
}

RE::TESObjectREFR* FollowerCollection::getRandomFollowerObjectFemale() {
    uint32_t rand;
    std::random_device rnd;
    rand = rnd() % getFollowersCountFemale();
    RE::TESObjectREFR* actor = _objectListFemale[rand];
    logs::info("FollowerCollection::getRandomFollowerObjectFemale\trand={}\tRE::Actor={:08X}", rand, actor->formID);
    return _objectListFemale[rand];
}
