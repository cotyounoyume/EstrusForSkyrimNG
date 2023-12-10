#pragma once
#include "PCH.h"
#include "Utility.h"

#include <stdio.h>
#include <fstream>
#include <random>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

//#include "skse64/GameData.h"
//#include "skse64/GameFormComponents.h"
//#include "skse64_common/Utilities.h"

#define GRUPSIZE 24
#define HEADERSIZE 24
#define UNITSIZE 4
#define HALFSIZE 2

struct TESFieldTarget {
    std::string type;
    uint32_t dataSize;
    uint32_t formID;
};

struct TESFieldUnknown {
    std::string type;
    uint32_t dataSize;
    char data[];
};

struct TESGroup {
    std::string type;
    uint32_t dataSize;
    std::string label;
    uint32_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;
    uint16_t unk5;
};

struct TESRecord {
    std::string type;
    uint32_t dataSize;
    uint32_t flags;
    uint32_t formID;
    uint32_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    char data[];
};

class TESReader {
public:
    static std::string readTESType(FILE*, uint32_t pos);
    static uint32_t readUInt32(FILE*, uint32_t pos);
    static uint32_t readUInt16(FILE*, uint32_t pos);
    static std::string readTESString(FILE* hFile, uint32_t pos, uint32_t length);
};

typedef struct _pair {
    uint32_t objID;
    uint32_t baseID;
    std::string modName;
} pair;

class TESFileReader : TESReader {
public:
    TESFileReader();
    TESFileReader(uint32_t);
    ~TESFileReader();

    std::vector<pair> getRefs();
    bool hasRefs();
    bool hasCells();
    bool isEsp();
    std::string getModName();
    void scanTESFileByModIndex();
    bool hasError();
    std::string getErrorType();
    void setUpperLimit(uint32_t);

private:
    std::string readTESType();
    uint32_t readUInt32();
    uint32_t readUInt16();
    std::string readTESString(uint32_t length);

    RE::TESDataHandler* _dhnd;
    FILE* _file;
    std::vector<pair> _pairs;
    std::vector<std::string> _masters;
    int _masterNum;
    std::string _modName;
    std::string _modType;
    uint32_t _pos;
    uint32_t _eofPos;
    uint32_t _cellEndPos;
    std::string _filePath;
    uint32_t _modIndex;
    bool _hasRefs;
    bool _hasCells;
    bool _hasBody;
    bool _isError;
    std::string _errorType;
    uint32_t _upperLimit;

    void _setFilePath();
    void _openTESFile();
    void _getTESHeader();
    void _skipTESHeader();
    void _skipToCellGroup();
    void _collectACHRFormIDs();
    void _closeTESFile();
    TESGroup _fetchNextTESGroup();
    TESRecord _fetchNextTESRecord();
    TESFieldUnknown _fetchNextTESFieldUnknown();
    TESFieldTarget _fetchNextTESFieldTarget();
    bool _ends_with(std::string, std::string);
    void _pushItem(uint32_t, uint32_t);
    uint32_t _modifyModIndexMask(uint32_t);
};

struct iniLine {
    std::string modName;
    uint32_t objectID;
    uint32_t formID;
    bool isFemale;
    std::string name;
};

struct iniConf {
    std::string key;
    std::string value;
};

class FollowerCollection {
public:
    FollowerCollection();
    ~FollowerCollection();

    static void collectAll();
    static RE::TESObjectREFR* getFollowerObjectMale(uint32_t);
    static RE::TESObjectREFR* getFollowerObjectFemale(uint32_t);
    static uint32_t getFollowersCountMale();
    static uint32_t getFollowersCountFemale();
    static RE::TESObjectREFR* getRandomFollowerObjectMale();
    static RE::TESObjectREFR* getRandomFollowerObjectFemale();

private:
    static RE::TESDataHandler* _dhnd;
    //	static std::unordered_map<RE::TESObjectREFR*, TESNPC*>	_objectMap;
    static std::vector<RE::TESObjectREFR*> _objectListMale;
    static std::vector<RE::TESObjectREFR*> _objectListFemale;
    static std::unordered_map<std::string, int> _dlcMap;
    static bool _enableFollowerFunction;
    static uint32_t _upperlimit;

    static void _readIniFile();
    static void _setupDLCsMap();
    static std::string _getIniFilePath();
    static iniLine _str2item(std::string);
    static iniConf _str2Conf(std::string);
    static bool _isDlc(std::string);
    static void _setconf(iniConf);
};
