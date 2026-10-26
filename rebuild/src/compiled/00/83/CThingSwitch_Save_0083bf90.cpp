// CThingSwitch::Save(CCharString& out) -- append the switch's level-script text after CThingPhysical's: "TriggerRadius <r>;", "TriggeredByThing <name|NULL>;", "EnvironmentDef <def|NULL>;" and "TimeToChangeEnvironmentDef <t>;", each ending "\r\n"
// Leaf: ?Save@CThingSwitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BF90, length 592 bytes (virtual, `ret 4`).
// Callees: CThingPhysical::Save 0x006A4BA0 (non-virtual), CCharString::FormatDouble(long, double) 0x009A08B0 (__fastcall,
//   hidden result in ecx), operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*)
//   0x0099F600, operator+(const CCharString&, const CCharString&) 0x0099F570, CCharString::operator+= 0x0099F0A0,
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0.
// Strings: "TriggerRadius " 0x01273068, "TriggeredByThing " 0x01273054, "EnvironmentDef " 0x01273044,
//   "TimeToChangeEnvironmentDef " 0x01273028, ";\r\n" 0x012353B8, "" 0x0122D70E, "NULL" 0x012393E4.
// Layout: Ego_r.pdb CThingSwitch: TriggerRadius +0xe8, EnvironmentDef +0xec, TimeToChangeEnvironmentDef +0xf0, Triggered +0xf4,
//   TriggerableObjectScriptName +0xf8; retail sits 0x18 lower (+0xd0 / +0xd4 / +0xd8 / +0xdc / +0xe0), as the landed
//   CThingSwitch::SerialiseGameState 0x0083C230 models; so a local model, not engine/CThingSwitch.h.
// Shape: the two name lines build their prefix as an explicit CCharString (0x0099F570 is CCharString + CCharString) and
//   pick `name == "" ? CCharString("NULL") : name`; the conditional temporaries are tracked by the ebx flag bits 1 and 2.
//   The inline CCharString::operator== has the null-block arm `strcmp(text, "") == 0` (the `repe cmpsb` against "").
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
    CCharString& operator+=(const CCharString& other);                          // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);      // 0x009A08B0
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
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);       // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570

class CThingPhysical { public:
    unsigned char _pad04[0xd0 - 4];
    virtual void Save(CCharString& out);                                        // 0x006A4BA0
};
class CThingSwitch : public CThingPhysical { public:
    float         TriggerRadius;                         // +0xd0
    CCharString   EnvironmentDef;                        // +0xd4
    float         TimeToChangeEnvironmentDef;            // +0xd8
    bool          Triggered;                             // +0xdc
    unsigned char _pad_dd[3];
    CCharString   TriggerableObjectScriptName;           // +0xe0
    virtual void Save(CCharString& out);
};

void CThingSwitch::Save(CCharString& out)
{
    CThingPhysical::Save(out);
    out += "TriggerRadius " + CCharString::FormatDouble(6, TriggerRadius) + ";\r\n";
    out += CCharString("TriggeredByThing ") + (TriggerableObjectScriptName == "" ? CCharString("NULL") : TriggerableObjectScriptName) + ";\r\n";
    out += CCharString("EnvironmentDef ") + (EnvironmentDef == "" ? CCharString("NULL") : EnvironmentDef) + ";\r\n";
    out += "TimeToChangeEnvironmentDef " + CCharString::FormatDouble(6, TimeToChangeEnvironmentDef) + ";\r\n";
}