// CThingPhysicalSwitch::Save(CCharString& out) -- append the switch's level-script text after CThingPhysical's: "TriggeredByThing <name|NULL>;\r\n"
// Leaf: ?Save@CThingPhysicalSwitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083D800, 243 bytes, virtual thiscall `ret 4` (CThingPhysicalSwitch vtable
//   0x01245644 slot 2).
// Identity: Ego_r.pdb ?Save@CThingPhysicalSwitch@@UAEXAAVCCharString@@@Z (the debug build's 0x00812A60); the catalog's
//   "CThingPhysical_SaveTriggeredByThing" is descriptive. Sibling of the landed CThingSwitch::Save 0x0083BF90.
// Callees: CThingPhysical::Save 0x006A4BA0 (non-virtual), CCharString(const char*, long) 0x0099EBF0,
//   operator+(const CCharString&, const char*) 0x0099F600, operator+(const CCharString&, const CCharString&) 0x0099F570,
//   CCharString::operator+= 0x0099F0A0, ~CCharString 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0.
// Strings: "" 0x0122D70E, "TriggeredByThing " 0x01273054, ";\r\n" 0x012353B8, "NULL" 0x012393E4.
// Layout: Ego_r.pdb CThingPhysicalSwitch: Triggered +0xe8, TriggerableObjectScriptName +0xec; retail sits 0x18 lower
//   (+0xd0 / +0xd4), the same shift as the landed CThingSwitch models; so a local model.
// Shape: the line starts from an explicit empty CCharString("") (the extra 0x0099EBF0 call before the first operator+),
//   then `name == "" ? CCharString("NULL") : name` (the conditional temporary tracked by ebx bit 1) as CThingSwitch::Save;
//   the inline CCharString::operator== has the null-block arm `strcmp(text, "") == 0` (the `repe cmpsb` against "").
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    ~CCharString();                                                             // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);                          // 0x0099F0A0
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
CCharString __fastcall operator+(const CCharString& left, const char* right);       // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570

class CThingPhysical { public:
    unsigned char _pad04[0xd0 - 4];
    virtual void Save(CCharString& out);                                        // 0x006A4BA0
};
class CThingPhysicalSwitch : public CThingPhysical { public:
    bool          Triggered;                             // +0xd0
    unsigned char _pad_d1[3];
    CCharString   TriggerableObjectScriptName;           // +0xd4
    virtual void Save(CCharString& out);
};

void CThingPhysicalSwitch::Save(CCharString& out)
{
    CThingPhysical::Save(out);
    out += CCharString("") + "TriggeredByThing " +
           (TriggerableObjectScriptName == "" ? CCharString("NULL") : TriggerableObjectScriptName) + ";\r\n";
}