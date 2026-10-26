// CThingFilter_IsInQuestOrNullQuest::operator()(const CThing& thing) const -- true when the thing's section is the filter's quest section, "NULL" or empty
// Leaf: ??RCThingFilter_IsInQuestOrNullQuest@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7C60, length 157 bytes, thiscall `ret 4`.
// Callees: Compare(const char*, const char*) @ 0x00411570 (__fastcall, the inlined CCharString equality),
//   CBasicString<char>::operator==(const char*) const @ 0x004115A0. Strings: "NULL" 0x012393E4, "" 0x0122D70E.
// Identity: Ego_r.pdb `??RCThingFilter_IsInQuestOrNullQuest@@QBE_NABVCThing@@@Z` (name set only); the filter
//   (one CCharString QuestSectionName, Ego_r) is bound with CThingFilter_OverlappingArea / _HasTC in the
//   CThingSearchTools instantiations that call this. The catalog's "CThingFilter_MatchesScriptName" is a
//   descriptive auto label.
// Layout: retail CThing SerialisationSectionName at +0x58 (Ego_r +0x68; the retail CThing is 0x10 smaller there:
//   interface map +0x44, see the landed TC readers), so a local model. CCharString = pointer to the shared
//   data block {chars, length, ...} (the landed CThingSwitch::Save 0x0083BF90 / OnThingWithScriptNameApplied
//   0x0083CC10 models).
// Shape: three inline CCharString equalities or-ed into an int-wide bool (`mov eax,1` / `xor eax,eax`): the
//   string == string form (same block, then both non-null with equal length and Compare() == 0), then two
//   string == literal forms whose null-block arm is the intrinsic `strcmp(text, "")` (`repe cmpsb`).
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

long __fastcall Compare(const char* a, const char* b);      // 0x00411570

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;                                    // +0x00
    unsigned long StringLength;                             // +0x04
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;                // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Data.StringLength == other.PStringData->Data.StringLength)
            return Compare(PStringData->Data.PData, other.PStringData->Data.PData) ? false : true;
        return false;
    }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CThing {
public:
    unsigned char _pad00[0x58];
    CCharString   SerialisationSectionName;                 // +0x58
};

class CThingFilter_IsInQuestOrNullQuest {
public:
    CCharString QuestSectionName;                           // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsInQuestOrNullQuest::operator()(const CThing& thing) const
{
    return thing.SerialisationSectionName == QuestSectionName
        || thing.SerialisationSectionName == "NULL"
        || thing.SerialisationSectionName == "";
}