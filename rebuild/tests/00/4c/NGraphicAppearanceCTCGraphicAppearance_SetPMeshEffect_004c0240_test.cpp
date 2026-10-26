// Behaviour test for 004c0240: final.cpp copied verbatim, the declared callees stubbed below so calls, arguments, the
// map entries and the effect reference counts are observable. NGraphicAppearance::CTCGraphicAppearance::SetPMeshEffect
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
#define free Test_free
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// NGraphicAppearance::CTCGraphicAppearance::SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>, bool, bool, bool, EAppearanceMeshEffectPriority, void*) -- register (or, for a null effect, clear) the owner's mesh effect on this appearance unless it is a weapon the effect must not touch, spread it to the graphic appearance of every carried thing when asked (weapons only when asked), then re-render
// Leaf: SetPMeshEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C0240, length 485 bytes, `ret 18h` (the by-value counted pointer is
//         destroyed by the callee). Ego_r.pdb `?SetPMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAEXV?$CIVCountedPointer@VCEngineMeshEffect@@@@_N11W4EAppearanceMeshEffectPriority@2@PAX@Z`.
// Callees: CTCGraphicAppearance::ClearMeshEffect(void*) @ 0x004BFE20 (landed), the MeshEffects map's operator[](const key&)
//          @ 0x004C5B90 (entry by reference), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCCarrying::GetAllCarriedThings @ 0x006A2220, itself recursively, free @ 0x00BFEA14, CThing::AddToRenderList
//          @ 0x004C7AC0, the effect's virtual slot 1 on release.
// Layout: as the landed SetLightingChannel 0x004BFB40 / ClearMeshEffect 0x004BFE20: ParentThing +0x04, MeshEffects +0x0c,
//          UpdatePrimitiveReasons +0x58; CMeshEffectPriorityEntry {PEffect, Priority +4, EffectsCarriedItems +8,
//          EffectsWeapons +9, InheritedFromCarrier +0xa}; retail CThing bitset +0x20 (weapon 0x48 = word 2 bit 8,
//          carrying 0x46 = bit 6, graphic appearance 0x5b = bit 27), interface map +0x44.
// Shape: the SetLightingChannel carried-thing walk with the presence tests in the bitset::test form through a named bool
//          (`shr ; and al,1`, the bool homed in a dead argument slot; the lane reproduces the homing); the parent's own test
//          reads the thing through a named local inside a __forceinline helper so the pointer's register dies at the word
//          load (`mov eax,[eax+28h]`; a direct member read keeps it in a second register, a local in the body hoists the
//          load above the branch); the entry is assigned from a temporary whose counted pointer is body-assigned
//          (`xor edi,edi ; test ebp,ebp` + load/inc/store), rides in edi and is released after the memberwise copy.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    void AddToRenderList();                                 // 0x004C7AC0
    bool TestTC(long interfaceType) const                   // std::bitset::test
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    bool IsWeapon() const                                   // the test's bool is a named local (stored)
    {
        bool weapon = TestTC(0x48);
        return weapon;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};

template <class T> struct CAllocator { CAllocator() { } };  // STLport allocator<T> (empty)
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
    T* begin() const { return this->_M_start; }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

class CTCCarrying { public:
    void GetAllCarriedThings(CRetailVector<CThing*>& things) const;   // 0x006A2220
};

class CEngineMeshEffect { public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template<class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CIVCountedPointer() { if (Object) Object->Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};
enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_DEFAULT };

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    struct CMeshEffectPriorityEntry {
        CIVCountedPointer<CEngineMeshEffect> PEffect;       // +0x0
        EAppearanceMeshEffectPriority        Priority;      // +0x4
        bool                                 EffectsCarriedItems;   // +0x8
        bool                                 EffectsWeapons;        // +0x9
        bool                                 InheritedFromCarrier;  // +0xa
        CMeshEffectPriorityEntry(const CIVCountedPointer<CEngineMeshEffect>& effect, EAppearanceMeshEffectPriority priority,
                                 bool effectsCarriedItems, bool effectsWeapons, bool inheritedFromCarrier)
            : Priority(priority), EffectsCarriedItems(effectsCarriedItems),
              EffectsWeapons(effectsWeapons), InheritedFromCarrier(inheritedFromCarrier) { PEffect = effect; }
    };
    struct CMeshEffectMap {                                 // STLport map<void*, entry>: header-node pointer, count, comparator
        void*         _M_header;
        unsigned long _M_node_count;
        unsigned long _M_compare;
        CMeshEffectPriorityEntry& operator[](void* const& key);   // 0x004C5B90
    };

    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    unsigned char  _pad18[0x58 - 0x18];
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    void ClearMeshEffect(void* owner);                      // 0x004BFE20
    __forceinline bool ParentIsWeapon() const               // the parent in a named local (its register dies at the word load)
    {
        CThing* parent = ParentThing;
        return parent->IsWeapon();
    }
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool effectsCarriedItems, bool effectsWeapons,
                        bool inheritedFromCarrier, EAppearanceMeshEffectPriority priority, void* owner);
};

void CTCGraphicAppearance::SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool effectsCarriedItems,
                                          bool effectsWeapons, bool inheritedFromCarrier,
                                          EAppearanceMeshEffectPriority priority, void* owner)
{
    if (effectsWeapons || !ParentIsWeapon()) {
        if (!effect.Object)
            ClearMeshEffect(owner);
        MeshEffects[owner] = CMeshEffectPriorityEntry(effect, priority, effectsCarriedItems, effectsWeapons, inheritedFromCarrier);
    }
    CTCCarrying* carrying;
    if (effectsCarriedItems && ParentThing->GetInterface(&carrying, 0x46)) {
        CRetailVector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned int i = 0; i < carried.size(); ++i) {
            CThing* thing = carried[i];
            CTCGraphicAppearance* appearance;
            if ((effectsWeapons || !thing->IsWeapon()) && thing->GetInterface(&appearance, 0x5b))
                appearance->SetPMeshEffect(effect, effectsCarriedItems, effectsWeapons, true, priority, owner);
        }
    }
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}

}
// ---- stubs ----
#undef free
using namespace NGraphicAppearance;
extern "C" void Test_free(void* p) { note("free"); free(p); }
static const char* ThingName(const CThing* t);
static const char* EffectName(const CEngineMeshEffect* e);
void CEngineMeshEffect::v00() { }
void CEngineMeshEffect::Destroy() { char b[64]; _snprintf(b, sizeof b, "destroy(%s)", EffectName(this)); note(b); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CThing* g_carried[8]; static int g_carriedCount = 0;
void CTCCarrying::GetAllCarriedThings(CRetailVector<CThing*>& things) const
{
    char b[48]; _snprintf(b, sizeof b, "carried(%d)", g_carriedCount); note(b);
    things._M_start = (CThing**)malloc(sizeof(CThing*) * (g_carriedCount ? g_carriedCount : 1));
    for (int i = 0; i < g_carriedCount; ++i) things._M_start[i] = g_carried[i];
    things._M_finish = things._M_start + g_carriedCount; things._M_end_of_storage.m_data = things._M_finish;
}
void CThing::AddToRenderList() { char b[64]; _snprintf(b, sizeof b, "render(%s)", ThingName(this)); note(b); }
void CTCGraphicAppearance::ClearMeshEffect(void* owner) { char b[64]; _snprintf(b, sizeof b, "clear(%s,%d)", ThingName(ParentThing), (int)(long)owner); note(b); }
struct SSlot { CTCGraphicAppearance::CMeshEffectMap* map; void* key; bool used; unsigned char raw[sizeof(CTCGraphicAppearance::CMeshEffectPriorityEntry)]; };
static SSlot g_slots[16];
CTCGraphicAppearance::CMeshEffectPriorityEntry& CTCGraphicAppearance::CMeshEffectMap::operator[](void* const& key)
{
    for (int i = 0; i < 16; ++i)
        if (g_slots[i].used && g_slots[i].map == this && g_slots[i].key == key)
            return *(CMeshEffectPriorityEntry*)g_slots[i].raw;
    for (int i = 0; i < 16; ++i)
        if (!g_slots[i].used) {
            g_slots[i].used = true; g_slots[i].map = this; g_slots[i].key = key; memset(g_slots[i].raw, 0, sizeof g_slots[i].raw);
            ++_M_node_count; note("newentry");
            return *(CMeshEffectPriorityEntry*)g_slots[i].raw;
        }
    return *(CMeshEffectPriorityEntry*)g_slots[15].raw;
}
static CTCGraphicAppearance::CMeshEffectPriorityEntry* FindEntry(CTCGraphicAppearance::CMeshEffectMap* map, void* key)
{
    for (int i = 0; i < 16; ++i)
        if (g_slots[i].used && g_slots[i].map == map && g_slots[i].key == key)
            return (CTCGraphicAppearance::CMeshEffectPriorityEntry*)g_slots[i].raw;
    return 0;
}
static CThing g_parent, g_childA, g_childB, g_childC;
static CEngineMeshEffect g_effect1, g_effect2;
static const char* ThingName(const CThing* t) { return t == &g_parent ? "parent" : t == &g_childA ? "A" : t == &g_childB ? "B" : t == &g_childC ? "C" : "?"; }
static const char* EffectName(const CEngineMeshEffect* e) { return e == &g_effect1 ? "e1" : e == &g_effect2 ? "e2" : "?"; }
static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
static void ClearBit(CThing& t, long type) { t.m_presentBits[type >> 5] &= ~(1UL << (type & 31)); }
int main()
{
    CTCCarrying carrying;
    CTCGraphicAppearance app, appB; memset(&app, 0, sizeof app); memset(&appB, 0, sizeof appB);
    memset(&g_parent, 0, sizeof g_parent); memset(&g_childA, 0, sizeof g_childA); memset(&g_childB, 0, sizeof g_childB); memset(&g_childC, 0, sizeof g_childC);
    app.ParentThing = &g_parent; appB.ParentThing = &g_childB;
    CInterfaceMap::SEntry parentEntries[2]; parentEntries[0].type = 0x46; parentEntries[0].component = (CTCBase*)&carrying; parentEntries[1].type = 0x5b; parentEntries[1].component = (CTCBase*)&app;
    g_parent.m_interfaces.first = parentEntries; g_parent.m_interfaces.last = parentEntries + 2;
    CInterfaceMap::SEntry bEntries[1]; bEntries[0].type = 0x5b; bEntries[0].component = (CTCBase*)&appB;
    g_childB.m_interfaces.first = bEntries; g_childB.m_interfaces.last = bEntries + 1;
    g_childA.m_interfaces = g_childB.m_interfaces; g_childC.m_interfaces.first = g_childC.m_interfaces.last = 0;
    SetBit(g_childA, 0x48); SetBit(g_childA, 0x5b); SetBit(g_childB, 0x5b);   // A: a weapon with an appearance, B: an appearance, C: nothing
    g_effect1.RefCount = 1; g_effect2.RefCount = 1;
    CIVCountedPointer<CEngineMeshEffect> e1; e1.Object = &g_effect1; g_effect1.RefCount = 2;
    CIVCountedPointer<CEngineMeshEffect> e2; e2.Object = &g_effect2; g_effect2.RefCount = 2;
    CIVCountedPointer<CEngineMeshEffect> none;
    // 1. parent is a weapon, effects must not touch weapons: no entry; no carried pass (effectsCarriedItems false)
    SetBit(g_parent, 0x48);
    g_log[0] = 0; app.SetPMeshEffect(e1, false, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)1);
    CHECKLOG("render(parent);"); CHECK(app.MeshEffects._M_node_count == 0 && g_effect1.RefCount == 2); CHECK((app.UpdatePrimitiveReasons & 4) != 0);
    // 2. weapons allowed: entry created and filled, the effect referenced once more
    g_log[0] = 0; app.SetPMeshEffect(e1, false, true, true, (EAppearanceMeshEffectPriority)3, (void*)1);
    CHECKLOG("newentry;render(parent);");
    { CTCGraphicAppearance::CMeshEffectPriorityEntry* en = FindEntry(&app.MeshEffects, (void*)1);
      CHECK(en && en->PEffect.Object == &g_effect1 && en->Priority == 3 && !en->EffectsCarriedItems && en->EffectsWeapons && en->InheritedFromCarrier); }
    CHECK(g_effect1.RefCount == 3);
    // 3. parent not a weapon: same owner re-set with the same effect (no ref change), new flags
    ClearBit(g_parent, 0x48);
    g_log[0] = 0; app.SetPMeshEffect(e1, false, false, false, (EAppearanceMeshEffectPriority)5, (void*)1);
    CHECKLOG("render(parent);");
    { CTCGraphicAppearance::CMeshEffectPriorityEntry* en = FindEntry(&app.MeshEffects, (void*)1);
      CHECK(en && en->PEffect.Object == &g_effect1 && en->Priority == 5 && !en->EffectsWeapons && !en->InheritedFromCarrier); }
    CHECK(g_effect1.RefCount == 3);
    // 4. replace with another effect: the old one released, the new one referenced
    g_log[0] = 0; app.SetPMeshEffect(e2, false, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)1);
    CHECKLOG("render(parent);"); CHECK(g_effect1.RefCount == 2 && g_effect2.RefCount == 3);
    CHECK(FindEntry(&app.MeshEffects, (void*)1)->PEffect.Object == &g_effect2);
    // 5. a null effect clears the owner first, then stores the null entry (the old effect released to the caller's count)
    g_log[0] = 0; app.SetPMeshEffect(none, false, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)1);
    CHECKLOG("clear(parent,1);render(parent);"); CHECK(g_effect2.RefCount == 2 && FindEntry(&app.MeshEffects, (void*)1)->PEffect.Object == 0);
    // 6. release to zero: a single-ref effect owned only by the entry is destroyed when replaced
    CEngineMeshEffect solo; solo.RefCount = 0;
    { CIVCountedPointer<CEngineMeshEffect> s; s.Object = &solo; solo.RefCount = 1;
      g_log[0] = 0; app.SetPMeshEffect(s, false, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)2);
      CHECKLOG("newentry;render(parent);"); CHECK(solo.RefCount == 2); s.Object = 0; --solo.RefCount; }
    g_log[0] = 0; app.SetPMeshEffect(e1, false, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)2);
    CHECKLOG("destroy(?);render(parent);"); CHECK(solo.RefCount == 0 && g_effect1.RefCount == 3);
    // 7. carried pass without the carrying component: nothing walked
    g_log[0] = 0; app.SetPMeshEffect(e1, true, false, false, MESH_EFFECT_PRIORITY_DEFAULT, (void*)3);
    CHECKLOG("newentry;render(parent);");
    // 8. carried pass: A (weapon) skipped, B recursed with inheritedFromCarrier, C (no appearance) skipped
    SetBit(g_parent, 0x46); g_carried[0] = &g_childA; g_carried[1] = &g_childB; g_carried[2] = &g_childC; g_carriedCount = 3;
    g_log[0] = 0; app.SetPMeshEffect(e2, true, false, false, (EAppearanceMeshEffectPriority)7, (void*)3);
    CHECKLOG("carried(3);newentry;render(B);free;render(parent);");
    { CTCGraphicAppearance::CMeshEffectPriorityEntry* en = FindEntry(&appB.MeshEffects, (void*)3);
      CHECK(en && en->PEffect.Object == &g_effect2 && en->Priority == 7 && en->EffectsCarriedItems && !en->EffectsWeapons && en->InheritedFromCarrier); }
    CHECK(g_effect2.RefCount == 4 && g_effect1.RefCount == 3);
    // 9. weapons included: A is recursed too (its interface map points at appB, whose parent is B), then B again
    g_log[0] = 0; app.SetPMeshEffect(e2, true, true, false, (EAppearanceMeshEffectPriority)8, (void*)3);
    CHECKLOG("carried(3);render(B);render(B);free;render(parent);");
    CHECK(FindEntry(&appB.MeshEffects, (void*)3)->Priority == 8 && g_effect2.RefCount == 4);
    // 10. an empty carried list: only the parent
    g_carriedCount = 0;
    g_log[0] = 0; app.SetPMeshEffect(e2, true, true, false, (EAppearanceMeshEffectPriority)8, (void*)3);
    CHECKLOG("carried(0);free;render(parent);");
    if (fails == 0) printf("004c0240_TEST PASS\n"); else printf("004c0240_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}