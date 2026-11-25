#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailWideTextEquals(const void* p, const char* want)   // ASCII text, compared unit by unit
{
    const unsigned short* s = (const unsigned short*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != (unsigned short)(unsigned char)*want) return false; ++s; ++want; }
    return *s == 0;
}
namespace N_0099eae0 { struct CWideString {
    void* p;
    CWideString(const char* s, int len){ p=(void*)s; }
    CWideString(const char* s){ p=(void*)s; }
    ~CWideString(){}
}; } using N_0099eae0::CWideString;
static const char* g_symbol = 0;   // NF410: the text-bank symbol the Format path receives
struct SaveSub { CWideString Format(CWideString* in){ g_symbol = (const char*)in->p; return CWideString((const char*)0xF07,0); } };
struct SaveMgr { char pad[0x14]; SaveSub* f14; };
SaveMgr* g_saveMgr;
class CUserProfileManager { public: CWideString GetEmptySlotName(); };
CWideString CUserProfileManager::GetEmptySlotName(){
    SaveMgr* mgr=g_saveMgr;
    if(mgr){ CWideString temp("TEXT_GUI_SAVE_EMPTY_SLOT",-1); return mgr->f14->Format(&temp); }
    return CWideString((const char*)L"<Empty Slot Name>");
}
int main(){
    CUserProfileManager u;
    g_saveMgr=0;
    CWideString r1=u.GetEmptySlotName();          // null -> literal factory
    static SaveSub ss; static SaveMgr mgr; SaveSub* psub=&ss; mgr.f14=psub; g_saveMgr=&mgr;
    CWideString r2=u.GetEmptySlotName();          // non-null -> Format path
    if(RetailWideTextEquals(r1.p, "<Empty Slot Name>") && r2.p==(void*)0xF07 && RetailTextEquals(g_symbol, "TEXT_GUI_SAVE_EMPTY_SLOT")){ printf("EMPTYSLOT_OK\n"); return 0; }
    printf("FAIL r1=%p r2=%p\n", r1.p, r2.p); return 1;
}