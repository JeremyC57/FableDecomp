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

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
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