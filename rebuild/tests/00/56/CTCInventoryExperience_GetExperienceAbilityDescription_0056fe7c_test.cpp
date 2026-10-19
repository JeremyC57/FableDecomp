// Behaviour test for retail 0x0056fe7c: the candidate source below is embedded verbatim (from src/0056fe7c.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level) -- resolve the ability's description text
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FE7C, 193 bytes.
// Callees: bitset<>::test 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the inlined
//   CThing::GetInterface lookup for interface 0x6f = the hero abilities component), CTCInventoryAbilities::GetAbilityPDef
//   0x005EDFD9 (returns a counted CDefPointer by value), CTCInventoryAbilities::GetAbilityMaximumLevel 0x005EDEA3,
//   CTCInventoryBase::GetInventoryText 0x005BD273 (inherited), CWideString copy ctor 0x0099B720 from the empty-string
//   static 0x013BCA24. The CDefPointer destructor releases the def through virtual slot 1 when its count reaches 0.
// Layout: CHeroAbilityDef description table pointer +0x60 and maxed-out description +0x6c (retail offsets);
//   CThing interface bitset +0x20 and interface map +0x44 as in the landed GetInterface users. Local model: the
//   engine/ headers for these classes do not declare the methods called here.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString { void* PStringData; CWideString(const CWideString&); ~CWideString(); };
extern CWideString g_EmptyWideString;                   // 0x013BCA24
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
struct CDefPointeeBase { virtual void VFunc00(); virtual void Release(); long RefCount; };
struct CHeroAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x58];
    long* Descriptions;                                 // +0x60
    unsigned char _pad64[8];
    long MaxedOutDescription;                           // +0x6c
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* p = (CDefPointeeBase*)Object; if (--p->RefCount == 0) p->Release(); } }
};
struct CTCBase;
struct CTCInventoryAbilities {
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    long GetAbilityMaximumLevel(EHeroAbility ability) const;                         // 0x005EDEA3
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
struct CThing {
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
struct CTCInventoryBase {
    void* __vftable; CThing* ParentThing;
    CWideString GetInventoryText(long text_id);         // 0x005BD273
};
struct CTCInventoryExperience : CTCInventoryBase {
    CWideString GetExperienceAbilityDescription(long ability, long level);
};
CWideString CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level)
{
    CTCInventoryAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0x6f))
        return g_EmptyWideString;
    CDefPointer<const CHeroAbilityDef> def = abilities->GetAbilityPDef((EHeroAbility)ability);
    long textId;
    if (level == abilities->GetAbilityMaximumLevel((EHeroAbility)ability) + 1)
        textId = def.Object->MaxedOutDescription;
    else
        textId = def.Object->Descriptions[level];
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_released, g_textcalls, g_maxlevel = 3, g_has_iface = 1, g_lastability; static long g_lasttext;
struct RawW { void* p; }; static RawW g_rawEmpty = { (void*)0x0e };
CWideString g_EmptyWideString(*(const CWideString*)&g_rawEmpty);
CWideString::CWideString(const CWideString& o) : PStringData(o.PStringData) {}
CWideString::~CWideString() {}
void CDefPointeeBase::VFunc00() {}
void CDefPointeeBase::Release() { ++g_released; }
static CHeroAbilityDef g_def; static long g_desc[4] = { 100, 0, 102, 103 };
CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility a) const {
    /* one reference for the caller, one for the named local the unoptimised stub destroys after copying it out */
    g_lastability = a; g_def.RefCount += 2; CDefPointer<const CHeroAbilityDef> r; r.Object = &g_def; return r; }
long CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) const { return g_maxlevel; }
static CTCInventoryAbilities g_abilities;
static CInterfaceMap::Entry g_entries[3] = { {0x10, 0}, {0x6f, &g_abilities}, {0x70, 0} };
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* k) { Entry* e = first; while (e != last && e->type < *k) ++e; return e; }
bool CPresenceBits::test(long kind) { return kind == 0x6f && g_has_iface; }
CWideString CTCInventoryBase::GetInventoryText(long id) { ++g_textcalls; g_lasttext = id; RawW raw = { (void*)id }; return *(const CWideString*)&raw; }
int main() {
    static CThing thing; thing.interfaces.first = g_entries; thing.interfaces.last = g_entries + 3;
    g_def.Descriptions = g_desc; g_def.MaxedOutDescription = 555; g_def.RefCount = 1;
    CTCInventoryExperience e; e.ParentThing = &thing;
    int ok = (char*)&thing.interfaces - (char*)&thing == 0x44 && (char*)&g_def.MaxedOutDescription - (char*)&g_def == 0x6c;
    { CWideString r = e.GetExperienceAbilityDescription(7, 2); ok = ok && r.PStringData == (void*)102 && g_lastability == 7; }
    ok = ok && g_def.RefCount == 1 && g_released == 0;
    { CWideString r = e.GetExperienceAbilityDescription(7, 4); ok = ok && r.PStringData == (void*)555 && g_lasttext == 555; }   /* max+1 */
    { CWideString r = e.GetExperienceAbilityDescription(7, 1); ok = ok && r.PStringData == (void*)0x0e && g_textcalls == 2; } /* id 0 */
    g_def.RefCount = 0;                                  /* the returned reference is the last one: released on exit */
    { CWideString r = e.GetExperienceAbilityDescription(7, 0); ok = ok && r.PStringData == (void*)100 && g_released == 1; }
    g_has_iface = 0;
    { CWideString r = e.GetExperienceAbilityDescription(7, 0); ok = ok && r.PStringData == (void*)0x0e && g_textcalls == 3; }
    printf(ok ? "0056fe7c_TEST PASS\n" : "0056fe7c_TEST FAIL\n"); return ok ? 0 : 1;
}