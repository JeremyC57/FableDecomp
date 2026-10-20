// Behaviour test for 00811100: the model below is a copy of src/00811100.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_compareCalls = 0, g_getPItemCalls = 0;
#pragma pack(push, 1)
struct CBasicStringChar {                          // CBasicString<char>
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
#pragma pack(pop)
long __fastcall CompareStrings(const char* a, const char* b) { ++g_compareCalls; return strcmp(a, b); }
class CCharString {
public:
    CBasicStringChar* PStringData;                 // +0x00 (the shared block starts with its CBasicString)
    __forceinline bool operator==(const CCharString& other) const
    {
        CBasicStringChar* a = PStringData;
        CBasicStringChar* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->StringLength == b->StringLength)
            return CompareStrings(a->PData, b->PData) == 0;
        return false;
    }
};
struct CThing {
    unsigned char _pad00[0x74];
    CCharString   ScriptName;                      // +0x74
    unsigned char _pad78[0x91 - 0x78];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _flags91 : 7;
};
static CThing* g_item = 0;
struct CBaseIntelligentPointer {
    CThing* GetPItem() const { ++g_getPItemCalls; return g_item; }
};
struct CHitRecord {
    unsigned char           _pad00[0x50];
    CBaseIntelligentPointer PAttacker;             // +0x50
    CBaseIntelligentPointer* GetAttacker() { return &PAttacker; }
};
struct CHitEvent {
    unsigned char _pad00[8];
    CHitRecord*   PRecord;                         // +0x08
};
struct CIsHitEventHitByThingWithScriptName {
    CCharString ScriptName;                        // +0x00
    bool operator()(const CHitEvent& hit) const;
};

bool CIsHitEventHitByThingWithScriptName::operator()(const CHitEvent& hit) const
{
    if (ScriptName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (*ScriptName.PStringData == "")
        goto empty;
    {
        CHitRecord* record = hit.PRecord;
        CThing* attacker = record->GetAttacker()->GetPItem();
        if (attacker && !attacker->Dead && attacker->ScriptName == ScriptName)
            goto empty;
    }
    return false;
empty:
    return true;
}

// ---- test doubles and driver ----
static CBasicStringChar* Block(const char* text) { CBasicStringChar* b = new CBasicStringChar; b->PData = (char*)text; b->StringLength = (unsigned long)strlen(text); return b; }
static bool Run(CBasicStringChar* filterName, CThing* attacker, bool expect, int expectGetPItem)
{
    CIsHitEventHitByThingWithScriptName f; f.ScriptName.PStringData = filterName;
    CHitRecord record; std::memset(&record, 0, sizeof(record));
    CHitEvent hit; std::memset(&hit, 0, sizeof(hit)); hit.PRecord = &record;
    g_item = attacker; g_getPItemCalls = 0;
    bool r = f(hit);
    return r == expect && g_getPItemCalls == expectGetPItem;
}
int main()
{
    bool ok = true;
    CThing bandit; std::memset(&bandit, 0, sizeof(bandit)); bandit.ScriptName.PStringData = Block("BanditLeader");
    CThing dead = bandit; dead.Dead = 1;
    CThing other; std::memset(&other, 0, sizeof(other)); other.ScriptName.PStringData = Block("Villager");
    ok = ok && Run(0, &other, true, 0);                            // null filter name: anything matches
    ok = ok && Run(Block(""), &other, true, 0);                    // empty filter name: anything matches
    ok = ok && Run(Block("BanditLeader"), &bandit, true, 1);       // same name (compared by content)
    ok = ok && Run(bandit.ScriptName.PStringData, &bandit, true, 1);   // same block
    ok = ok && Run(Block("BanditLeader"), &dead, false, 1);        // dead attacker
    ok = ok && Run(Block("BanditLeader"), &other, false, 1);       // other name
    ok = ok && Run(Block("BanditLeader"), 0, false, 1);            // no attacker
    ok = ok && Run(Block("Bandit"), &bandit, false, 1);            // different length: no compare
    if (!ok) { std::printf("00811100_TEST FAIL\n"); return 1; }
    std::printf("00811100_TEST PASS\n");
    return 0;
}