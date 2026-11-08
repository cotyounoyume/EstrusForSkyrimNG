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
    char cstr[256] = {};
    if (length >= sizeof(cstr)) {
        length = sizeof(cstr) - 1;
    }
    fseek(hFile, pos, 0);
    fread(cstr, sizeof(char), length, hFile);
    return cstr;
}

uint32_t TESReader::readUInt32(FILE* hFile, uint32_t pos) {
    uint32_t value = 0;
    fseek(hFile, pos, 0);
    fread(&value, sizeof(value), 1, hFile);
    return value;
}

uint32_t TESReader::readUInt16(FILE* hFile, uint32_t pos) {
    uint16_t value = 0;
    fseek(hFile, pos, 0);
    fread(&value, sizeof(value), 1, hFile);
    return value;
}

TESFileReader::TESFileReader() : TESFileReader(nullptr) {}

TESFileReader::TESFileReader(const RE::TESFile* a_file) {
    _file = nullptr;
    _modIndex = 0;
    _pos = 0;
    _eofPos = 0;
    _cellEndPos = 0;
    _hasRefs = false;
    _hasCells = false;
    _isError = false;
    _hasBody = true;
    _errorType = "";
    _upperLimit = 0;
    _masterNum = 0;
    _dhnd = RE::TESDataHandler::GetSingleton();

    _setFilePath(a_file);
}

TESFileReader::~TESFileReader() { _closeTESFile(); }

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
        return 0;
    }
    return TESReader::readUInt32(_file, _pos);
}

uint32_t TESFileReader::readUInt16() {
    if (_pos >= _eofPos) {
        return 0;
    }
    return TESReader::readUInt16(_file, _pos);
}



void TESFileReader::_setFilePath(const RE::TESFile* a_file) {
    if (!a_file) {
        _isError = true;
        _errorType = "no mod file";
        return;
    }
    _modName = a_file->GetFilename();
    _filePath = Utility::GetCurrentExecutableDirectory() + "\\data\\" + _modName;
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
    if (_isError) {
        return;
    }
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
    if (fopen_s(&_file, _filePath.c_str(), "rb") != 0 || !_file) {
        logs::info("can't open file {}", _filePath.c_str());
        _file = nullptr;
        _isError = true;
        _errorType = "can't open file";
        return;
    } else {
        fpos_t fsize = 0;

        fseek(_file, 0, SEEK_END);
        fgetpos(_file, &fsize);
        if (fsize < 0 || fsize > 0xFFFFFFFFll) {
            _closeTESFile();
            _isError = true;
            _errorType = "unsupported file size";
            return;
        }
        _eofPos = static_cast<uint32_t>(fsize);
        if (_upperLimit == 0 || _upperLimit > fsize) {
            return;
        } else {
            _closeTESFile();
            _isError = true;
            _errorType = "skip for file size upper limit";
            logs::info("TESFileReader::{}'s file size({}) exceeds the upper limit({})", _filePath.c_str(), fsize, _upperLimit);
            return;
        }
        // logs::info("TESFileReader::%s: %d bytes", _filePath.c_str(), _eofPos);
    }
}

void TESFileReader::_closeTESFile() {
    if (_file) {
        fclose(_file);
        _file = nullptr;
    }
}

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
        if (tg.type != "GRUP" || tg.dataSize < GRUPSIZE) {
            // Broken or unexpected data. Stop here so we don't loop forever.
            break;
        }
        if (tg.label == "CELL") {
            isSuccess = true;
            _cellEndPos = (std::min)(_pos + tg.dataSize, _eofPos);
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
                if (tr.flags & 0x00040000) {
                    // Compressed record: the NAME field can't be read directly.
                    _pos = nextPos;
                    tg = _fetchNextTESGroup();
                    continue;
                }

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
                uint32_t objID = _resolveFormID(tr.formID);
                uint32_t baseID = _resolveFormID(abformID);
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

// Convert a form ID as written in the plugin file (top byte = master index)
// to the runtime form ID. The engine helper handles light (ESL) masters too.
uint32_t TESFileReader::_resolveFormID(uint32_t formID) {
    if (formID == 0 || !_dhnd) {
        return 0;
    }
    return _dhnd->LookupFormIDRaw(formID, _modName);
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

bool FollowerCollection::_isDlc(std::string modName) {
    return modName == "Skyrim.esm" || _dlcMap.contains(modName);
}

void FollowerCollection::_setupDLCsMap() {
    RE::TESDataHandler* dhnd = RE::TESDataHandler::GetSingleton();
    _dlcMap.clear();
    for (const char* name : { "Skyrim.esm", "Dawnguard.esm", "HearthFires.esm", "Dragonborn.esm" }) {
        if (dhnd && dhnd->LookupModByName(name)) {
            _dlcMap[name] = 1;
        }
    }
}

iniConf FollowerCollection::_str2Conf(std::string str) {
    std::vector<std::string> items = split(str, '\t');
    iniConf item;
    item.key = items.size() > 0 ? items[0] : "";
    item.value = items.size() > 1 ? items[1] : "";
    return item;
}

iniLine FollowerCollection::_str2item(std::string str) {
    std::vector<std::string> items = split(str, '\t');
    iniLine item;
    item.formID = 0;
    item.objectID = 0;
    item.isFemale = false;
    if (items.size() < 5) {
        return item;
    }
    item.modName = items[0];
    item.isFemale = (items[3] == "Female");
    item.name = items[4];

    if (!_isDlc(item.modName)) {
        return item;
    }

    auto dhnd = RE::TESDataHandler::GetSingleton();
    auto file = dhnd ? dhnd->LookupModByName(item.modName) : nullptr;
    if (!file) {
        return item;
    }
    const uint32_t mask = file->IsLight() ? 0xFFF : 0xFFFFFF;
    item.objectID = dhnd->LookupFormID(std::strtoul(items[1].c_str(), NULL, 16) & mask, item.modName);
    item.formID = dhnd->LookupFormID(std::strtoul(items[2].c_str(), NULL, 16) & mask, item.modName);

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
        try {
            _upperlimit = static_cast<uint32_t>(std::stoul(conf.value));
        } catch (const std::exception&) {
            logs::info("invalid FILE_UPPER_LIMIT_SIZE: {}", conf.value);
        }
    } else if (conf.key == "GET_FOLLOWER_FROM_OTHER_MOD") {
        if (conf.value == "OFF") {
            _enableFollowerFunction = false;
        }
    }
}

void FollowerCollection::collectAll() {
    RE::TESDataHandler* dhnd = RE::TESDataHandler::GetSingleton();
    if (!dhnd) {
        logs::info("FollowerCollection::collectAll:\tTESDataHandler is not ready");
        return;
    }
    _objectListMale.clear();
    _objectListFemale.clear();

    _readIniFile();
    if (!_enableFollowerFunction) {
        logs::info("FollowerCollection::collectionAll:\tGET_FOLLOWER_FROM_OTHER_MOD OFF");
        return;
    }

    const uint32_t modCount = dhnd->GetLoadedModCount();
    for (uint32_t i = 0; i < modCount; i++) {
        _scanMod(dhnd->LookupLoadedModByIndex(static_cast<uint8_t>(i)));
    }
    const uint32_t lightModCount = dhnd->GetLoadedLightModCount();
    for (uint32_t i = 0; i < lightModCount; i++) {
        _scanMod(dhnd->LookupLoadedLightModByIndex(static_cast<uint16_t>(i)));
    }
}

void FollowerCollection::_scanMod(const RE::TESFile* a_file) {
    if (!a_file) {
        return;
    }
    TESFileReader trf(a_file);
    if (_isDlc(trf.getModName())) {
        logs::info("FollowerCollection::collectionAll():\t{} is Default DLC file", trf.getModName());
        return;
    }
    trf.setUpperLimit(_upperlimit);
    trf.scanTESFileByModIndex();

    for (const auto& item : trf.getRefs()) {
        RE::TESNPC* npc = RE::TESForm::LookupByID<RE::TESNPC>(item.baseID);
        if (!npc) {
            continue;
        }
        RE::TESObjectREFR* objRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(item.objID);
        const char* fullName = npc->GetFullName();
        std::string npcName = fullName ? fullName : "";
        for (const auto& factionRank : npc->factions) {
            // 0x0005C84D: PotentialFollowerFaction (Skyrim.esm)
            if (!factionRank.faction || factionRank.faction->formID != 0x0005C84D) {
                continue;
            }
            if (objRef) {
                logs::info("FollowerCollection::collectionAll:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\texist",
                           item.objID, item.baseID, item.modName, npcName);
                if (npc->GetSex() == RE::SEXES::kFemale) {
                    _objectListFemale.push_back(objRef);
                } else {
                    _objectListMale.push_back(objRef);
                }
            } else {
                logs::info("FollowerCollection::collectionAll:\t{:08X}\t{:08X}\tfollower\t{:40s}\t{}\tNOT EXIST",
                           item.objID, item.baseID, item.modName, npcName);
            }
            break;
        }
    }
}

uint32_t FollowerCollection::getFollowersCountMale() { return static_cast<uint32_t>(_objectListMale.size()); }

RE::TESObjectREFR* FollowerCollection::getFollowerObjectMale(uint32_t num) {
    if (num >= _objectListMale.size()) {
        logs::info("FollowerCollection::getFollowerObjectMale\tnum={} is out of range ({})", num, _objectListMale.size());
        return nullptr;
    }
    logs::info("FollowerCollection::getFollowerObjectMale\tnum={}\tRE::Actor={:08X}", num, _objectListMale[num]->formID);
    return _objectListMale[num];
}

RE::TESObjectREFR* FollowerCollection::getRandomFollowerObjectMale() {
    if (_objectListMale.empty()) {
        return nullptr;
    }
    std::random_device rnd;
    uint32_t rand = rnd() % getFollowersCountMale();
    RE::TESObjectREFR* actor = _objectListMale[rand];
    logs::info("FollowerCollection::getRandomFollowerObjectMale\trand={}\tRE::Actor={:08X}", rand, actor->formID);
    return actor;
}

uint32_t FollowerCollection::getFollowersCountFemale() { return static_cast<uint32_t>(_objectListFemale.size()); }

RE::TESObjectREFR* FollowerCollection::getFollowerObjectFemale(uint32_t num) {
    if (num >= _objectListFemale.size()) {
        logs::info("FollowerCollection::getFollowerObjectFemale\tnum={} is out of range ({})", num, _objectListFemale.size());
        return nullptr;
    }
    logs::info("FollowerCollection::getFollowerObjectFemale\tnum={}\tRE::Actor={:08X}", num, _objectListFemale[num]->formID);
    return _objectListFemale[num];
}

RE::TESObjectREFR* FollowerCollection::getRandomFollowerObjectFemale() {
    if (_objectListFemale.empty()) {
        return nullptr;
    }
    std::random_device rnd;
    uint32_t rand = rnd() % getFollowersCountFemale();
    RE::TESObjectREFR* actor = _objectListFemale[rand];
    logs::info("FollowerCollection::getRandomFollowerObjectFemale\trand={}\tRE::Actor={:08X}", rand, actor->formID);
    return actor;
}
