// CHeroExperienceDef::CHeroExperienceDef() -- the definition base, then three empty long vectors, the seven std::map members (out-of-line constructors) and an empty AttackHistoryCombos vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB9E8, length 128 bytes.
// Label: the manifest's "C3DMeshMaterial::C3DMeshMaterial" is propagated; the vtable stored here, 0x0124390C,
// is CHeroExperienceDef (rtti-vtables.tsv), and the members match the PDB CHeroExperienceDef laid out with
// retail's 12-byte std::vector / std::map (PDB +0x28.. sits 4 bytes per container high).
// Callees: 0x0044C0C0 the definition base constructor (propagated label CShoveHaPennyRegionDef), 0x004DF84E
// map<long,CDefString> ctor, 0x004EB953 map<long,CComboMultiplierData> ctor, 0x004EB971
// map<EDamageAttribute,CHeroStatIncreaseData> ctor (each builds its comparator/allocator temporaries itself).
// Leaf: ??0CHeroExperienceDef
// Shape: a size-optimised module (unaligned entry, `#pragma optimize("s", on)`); every store follows the
// vtable store, so the vectors are member objects with inline constructors. They must be the real STLport
// vector (`_Vector_base(const allocator&)` nulling start/finish and an `_STLP_alloc_proxy` end pointer built
// from an `allocator_type()` temporary): with three plain zeroing words the `lea ecx,[esi+4Ch]` of the first
// map is scheduled before the last vector store (the wave-25 "24i" fix). The last vector goes through `lea eax`.
#pragma optimize("s", on)

struct CDefinitionBase {                               // 0x28 bytes, polymorphic
    virtual void v00();
    CDefinitionBase();                                 // 0x0044C0C0
    unsigned char _body[0x24];
};

struct SAllocator { SAllocator() {} };                 // empty allocator (user constructor: no zero fill of the temporary)
template<class T> struct SAllocProxy : SAllocator {    // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};
template<class T> struct CVectorBase {                 // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};
template<class T> struct CRetailVector : CVectorBase<T> {   // retail std::vector (12 bytes)
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
};

class CDefString;
class CComboMultiplierData;
class CHeroStatIncreaseData;
class CAttackHistoryCombo;
struct CMapLongDefString           { CMapLongDefString();           void* _a; void* _head; long _size; };   // 0x004DF84E
struct CMapLongComboMultiplierData { CMapLongComboMultiplierData(); void* _a; void* _head; long _size; };   // 0x004EB953
struct CMapStatIncreasePerHitType  { CMapStatIncreasePerHitType();  void* _a; void* _head; long _size; };   // 0x004EB971

class CHeroExperienceDef : public CDefinitionBase { public:
    CRetailVector<long>                 ExperienceLevelThresholds;     // +0x28
    CRetailVector<long>                 HeroLevelTitles;               // +0x34
    CRetailVector<long>                 FallbackMultipliers;           // +0x40
    CMapLongDefString                   SoundIntensityThresholds;      // +0x4c
    CMapLongComboMultiplierData         ComboMultiplierMap;            // +0x58
    float                               ExperienceMultiplierParamPerUnitComboMultiplier;   // +0x64
    float                               ExperienceMultiplierRegionChangeMultiplier;        // +0x68
    CMapLongDefString                   ComboMultiplierIncrease;       // +0x6c
    CMapLongDefString                   ComboMultiplierLock;           // +0x78
    CMapLongDefString                   ComboMultiplierLost;           // +0x84
    CMapLongDefString                   ComboMultiplierRunningHitsIncrease;   // +0x90
    CMapStatIncreasePerHitType          StatIncreasePerHitType;        // +0x9c
    CRetailVector<CAttackHistoryCombo>  AttackHistoryCombos;           // +0xa8   sizeof == 0xb4
    virtual void v00();
    CHeroExperienceDef();
};

CHeroExperienceDef::CHeroExperienceDef()
{
}