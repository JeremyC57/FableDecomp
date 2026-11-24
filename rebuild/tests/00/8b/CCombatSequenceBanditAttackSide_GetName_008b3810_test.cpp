// NF325 (2026-11-24): the row now defines the PDB member CCombatSequence_BanditAttackSide::GetName() const (by-value CCharString
// built through CCharString::CCharString(const char*, long)), so this test calls that member (qualified, no vftable) and records
// the constructor's arguments. Was: the stand-in alias GetName(CCharString*) calling CCharString::Init.
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITATTACKSIDE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditAttackSide.h"
#include <stdio.h>
#include <string.h>

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};

static const char* g_last_s;
static long g_last_n;
static CCharString* g_last_self;

CCharString::CCharString(const char* s, long n)
{
    g_last_self = this; g_last_s = s; g_last_n = n; m_data = (char*)s;
}

int main()
{
    static char storage[sizeof(CCombatSequence_BanditAttackSide)];
    memset(storage, 0, sizeof(storage));
    const CCombatSequence_BanditAttackSide* obj = (const CCombatSequence_BanditAttackSide*)storage;
    CCharString out = obj->CCombatSequence_BanditAttackSide::GetName();
    int ok = g_last_s == (const char*)0x012787b8 && g_last_n == -1 && out.m_data == (char*)0x012787b8;
    if (ok) printf("GETNAME_BANDITATTACKSIDE_OK\n");
    else printf("FAIL s=%p n=%ld self=%p\n", g_last_s, g_last_n, g_last_self);
    return ok ? 0 : 1;
}
