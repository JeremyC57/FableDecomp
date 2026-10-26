// Behaviour test: CThingSwitch::Save over a small heap CCharString (C++03; g++ and VC7.1). The string storage is leaked
// on purpose (the model has no copy constructor, so the conditional's copies stay shallow).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) {}                                          // test only
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

static CCharStringData* MakeData(const char* text)
{
    CCharStringData* data = (CCharStringData*)malloc(sizeof(CCharStringData));
    unsigned long length = (unsigned long)strlen(text);
    data->Data.PData = (char*)malloc(length + 1);
    memcpy(data->Data.PData, text, length + 1);
    data->Data.StringLength = length;
    data->NoRefs = 1;
    return data;
}
static const char* Text(const CCharString& s) { return s.PStringData ? s.PStringData->Data.PData : ""; }
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CCharString::CCharString(const char* text, long length) : PStringData(0)
{
    if (length < 0 && *text) PStringData = MakeData(text);
}
CCharString::~CCharString() {}
CCharString& CCharString::operator+=(const CCharString& other)
{
    char buffer[512];
    strcpy(buffer, Text(*this));
    strcat(buffer, Text(other));
    PStringData = MakeData(buffer);
    return *this;
}
CCharString __fastcall CCharString::FormatDouble(long digits, double value)
{
    char buffer[64];
    sprintf(buffer, "%.*f", (int)digits, value);
    return CCharString(buffer);
}
static CCharString Join(const char* left, const char* right)
{
    char buffer[512];
    strcpy(buffer, left);
    strcat(buffer, right);
    return CCharString(buffer);
}
CCharString __fastcall operator+(const char* left, const CCharString& right) { return Join(left, Text(right)); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Join(Text(left), right); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { return Join(Text(left), Text(right)); }
void CThingPhysical::Save(CCharString& out) { out += CCharString("Health 1;\r\n"); }

int main()
{
    CThingSwitch sw;
    sw.TriggerRadius = 2.5f;
    sw.EnvironmentDef = CCharString("ENV_DUSK");
    sw.TimeToChangeEnvironmentDef = 3.0f;
    sw.TriggerableObjectScriptName = CCharString("");
    CCharString out("");
    sw.Save(out);
    const char* expect = "Health 1;\r\nTriggerRadius 2.500000;\r\nTriggeredByThing NULL;\r\nEnvironmentDef ENV_DUSK;\r\n"
                         "TimeToChangeEnvironmentDef 3.000000;\r\n";
    if (strcmp(Text(out), expect) != 0) { printf("FAIL 1: %s\n", Text(out)); return 1; }
    sw.TriggerableObjectScriptName = CCharString("LEVER_01");
    sw.EnvironmentDef = CCharString("");
    CCharString out2("");
    sw.Save(out2);
    const char* expect2 = "Health 1;\r\nTriggerRadius 2.500000;\r\nTriggeredByThing LEVER_01;\r\nEnvironmentDef NULL;\r\n"
                          "TimeToChangeEnvironmentDef 3.000000;\r\n";
    if (strcmp(Text(out2), expect2) != 0) { printf("FAIL 2: %s\n", Text(out2)); return 1; }
    printf("0083bf90_TEST PASS\n");
    return 0;
}