// Behaviour test for 00665510: src/00665510.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::UseAbility(long abilityDefIndex, float buttonPressure)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::UseAbility(long abilityDefIndex, float buttonPressure) -- pass the button pressure to the ranged-attacks component (if any), fetch the ability's def and hand it to the creature's virtual ability handler
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665510, 135 bytes (`ret 8`).
// Callees: CInterfaceMap::LowerBound @ 0x0040F020, CTCHeroRangedAttacks::SetButtonPressure(float) @ 0x007899E0,
//   CThing::GetDefinitionManager @ 0x004C7990 (label "CViewBase::GetDisplayEngine" propagated),
//   CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) @ 0x0043FFA0 (byte-twin label
//   GetDef<CThingPhysicalSwitchDef>), CThingCreatureBase virtual slot 72 (+0x120, 0x0066A4E0, manifest "vfunc_72").
// Layout: retail CThing: interface bitset +0x20 (TC 0x81 = CTCHeroRangedAttacks: word +0x30 bit 1), CVectorMap +0x44.
//   The def is ref counted (count +4, virtual Destroy slot 1).
// Shape: the ranged component through the out-pointer GetInterface (`mov eax,[eax+4] ; push ecx ; mov ecx,eax`; the
//   HasTC + GetTC spelling loads it straight into ecx); the def pointer lives in the dead pressure argument slot; its release is the inline CDefPointer dtor
//   (`dec [esi+4] ; jne ; call [vtbl+4]`).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work LowerBound does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the LowerBound instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    long r = TmplSink_s7_0(this, 0 + (type ? *(const long*)type : 0));
    return (SEntry*)r;
}
#pragma auto_inline(on)
class CTCHeroRangedAttacks { public:
    void SetButtonPressure(float pressure);     // 0x007899E0
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
class CHeroAbilityDef : public CDefClassBase { public: };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template<class T> struct CDefHolder {          // releases a def taken out of a CDefPointer slot (kept in one register)
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
};
class CDefinitionManager { public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out);   // 0x0043FFA0
};
void* FindDef_s7r_0();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const T>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    CDefinitionManager* GetDefinitionManager(); // 0x004C7990
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

class CThingCreatureBase : public CThing { public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void UseAbilityDef(const CHeroAbilityDef* def);    // slot 72 (+0x120) 0x0066A4E0
    void UseAbility(long abilityDefIndex, float buttonPressure);
    __forceinline void UseAbilityByIndex(long abilityDefIndex)
    {
        CDefPointer<const CHeroAbilityDef> out;
        GetDefinitionManager()->GetDef(abilityDefIndex, &out);
        CDefHolder<const CHeroAbilityDef> def(out.Object);
        UseAbilityDef(def.Object);
    }
};

void CThingCreatureBase::UseAbility(long abilityDefIndex, float buttonPressure)
{
    CTCHeroRangedAttacks* ranged;
    if (GetInterface(&ranged, 0x81))
        ranged->SetButtonPressure(buttonPressure);
    UseAbilityByIndex(abilityDefIndex);
}

// ---- stubs and driver ----
void CDefClassBase::v00() {}
void CThing::v00() {}
void CThingCreatureBase::v01() {}
void CThingCreatureBase::v02() {}
void CThingCreatureBase::v03() {}
void CThingCreatureBase::v04() {}
void CThingCreatureBase::v05() {}
void CThingCreatureBase::v06() {}
void CThingCreatureBase::v07() {}
void CThingCreatureBase::v08() {}
void CThingCreatureBase::v09() {}
void CThingCreatureBase::v10() {}
void CThingCreatureBase::v11() {}
void CThingCreatureBase::v12() {}
void CThingCreatureBase::v13() {}
void CThingCreatureBase::v14() {}
void CThingCreatureBase::v15() {}
void CThingCreatureBase::v16() {}
void CThingCreatureBase::v17() {}
void CThingCreatureBase::v18() {}
void CThingCreatureBase::v19() {}
void CThingCreatureBase::v20() {}
void CThingCreatureBase::v21() {}
void CThingCreatureBase::v22() {}
void CThingCreatureBase::v23() {}
void CThingCreatureBase::v24() {}
void CThingCreatureBase::v25() {}
void CThingCreatureBase::v26() {}
void CThingCreatureBase::v27() {}
void CThingCreatureBase::v28() {}
void CThingCreatureBase::v29() {}
void CThingCreatureBase::v30() {}
void CThingCreatureBase::v31() {}
void CThingCreatureBase::v32() {}
void CThingCreatureBase::v33() {}
void CThingCreatureBase::v34() {}
void CThingCreatureBase::v35() {}
void CThingCreatureBase::v36() {}
void CThingCreatureBase::v37() {}
void CThingCreatureBase::v38() {}
void CThingCreatureBase::v39() {}
void CThingCreatureBase::v40() {}
void CThingCreatureBase::v41() {}
void CThingCreatureBase::v42() {}
void CThingCreatureBase::v43() {}
void CThingCreatureBase::v44() {}
void CThingCreatureBase::v45() {}
void CThingCreatureBase::v46() {}
void CThingCreatureBase::v47() {}
void CThingCreatureBase::v48() {}
void CThingCreatureBase::v49() {}
void CThingCreatureBase::v50() {}
void CThingCreatureBase::v51() {}
void CThingCreatureBase::v52() {}
void CThingCreatureBase::v53() {}
void CThingCreatureBase::v54() {}
void CThingCreatureBase::v55() {}
void CThingCreatureBase::v56() {}
void CThingCreatureBase::v57() {}
void CThingCreatureBase::v58() {}
void CThingCreatureBase::v59() {}
void CThingCreatureBase::v60() {}
void CThingCreatureBase::v61() {}
void CThingCreatureBase::v62() {}
void CThingCreatureBase::v63() {}
void CThingCreatureBase::v64() {}
void CThingCreatureBase::v65() {}
void CThingCreatureBase::v66() {}
void CThingCreatureBase::v67() {}
void CThingCreatureBase::v68() {}
void CThingCreatureBase::v69() {}
void CThingCreatureBase::v70() {}
void CThingCreatureBase::v71() {}
static CInterfaceMap::SEntry* g_hit = 0; static long g_key = 0;
long TmplSink_s7_0(const void* self, long seen) { g_key = seen; return (long)g_hit; }
static void* g_def = 0;
long TmplSink_s7_1(const void* self, long seen) { return (long)g_def; }
static int g_released = 0; void ReleaseDef_s7(void* def) { ++g_released; }
static float g_pressure = -1; static CTCHeroRangedAttacks* g_ranged = 0;
void CTCHeroRangedAttacks::SetButtonPressure(float pressure) { g_ranged = this; g_pressure = pressure; }
static CDefinitionManager g_dm;
CDefinitionManager* CThing::GetDefinitionManager() { return &g_dm; }
static int g_destroyed = 0; void CDefClassBase::Destroy() { ++g_destroyed; }
static const CHeroAbilityDef* g_used = 0; static int g_uses = 0;
void CThingCreatureBase::UseAbilityDef(const CHeroAbilityDef* def) { ++g_uses; g_used = def; }
int main()
{
    static CThingCreatureBase creature; static CTCHeroRangedAttacks ranged; static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x81; entries[0].component = (CTCBase*)&ranged; entries[1].type = 0; creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 1;
    static CHeroAbilityDef def; def.RefCount = 1; g_def = &def;
    creature.UseAbility(12, 0.75f);                                   // no ranged component
    CHECK(g_ranged == 0); CHECK(g_uses == 1); CHECK(g_used == &def); CHECK(def.RefCount == 0); CHECK(g_destroyed == 1);
    creature.m_presentBits[0x81 >> 5] |= 1UL << (0x81 & 31); g_hit = &entries[0]; def.RefCount = 2;
    creature.UseAbility(12, 0.75f);
    CHECK(g_key == 0x81); CHECK(g_ranged == &ranged); CHECK(g_pressure == 0.75f); CHECK(g_uses == 2); CHECK(def.RefCount == 1); CHECK(g_destroyed == 1);
    g_def = 0; creature.UseAbility(3, 0.5f); CHECK(g_uses == 3); CHECK(g_used == 0); CHECK(g_pressure == 0.5f);   // no def: null handed on
    g_hit = creature.m_interfaces.last; creature.UseAbility(3, 0.25f); CHECK(g_ranged == 0 || g_pressure == 0.25f);
    if (fails == 0) printf("00665510_TEST PASS\n"); else printf("00665510_TEST FAIL\n");
    return fails;
}