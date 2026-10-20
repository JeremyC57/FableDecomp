// CTCFlammable::ExtinguishFire() -- reset every fire point, stop the parent's fire sound, reset the burning state from the def, rebuild the fire points and clear the parent as an opinion source
// Leaf: ExtinguishFire
//
// Retail 0x00780C80, 145 bytes (plain `ret`). Ego_r.pdb `?ExtinguishFire@CTCFlammable@@QAEXXZ`. Callees:
// std::for_each<CFirePoint*, mem_fun_ref_t<void, CFirePoint>> @ 0x00781370 (`__fastcall`, the
// returned functor's hidden pointer in ecx, first in edx; manifest "_For_each<..., CAddDefNamesToComboBox>"
// is a byte-twin instantiation), NFire::CFirePoint::Reset @ 0x00780700 (pushed as the functor's member
// pointer), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of the
// sound component, interface 16), the sound component's stop @ 0x0066AEB0 (manifest
// "CTCSummonSpell::ResumeForCutscene", propagated), CTCFlammable::InitialiseFirePoints() @ 0x007808B0,
// CTCBase::GetWorld() @ 0x00686A70, COpinionReactionManager::ClearOpinionSource(CThing*, bool) @
// 0x007008C0 (the world's +0x74 manager).
// Layout: as the landed CTCFlammable::OnInitialActivate (0x00780D20): PDef +0xc, FirePoints +0x10
// (12-byte retail vector), Burning +0x1c, FireDamageAfterFrame +0x20, FireResistance +0x24,
// FireStrength +0x28, DamageCaused +0x2c, DamagePerPoint +0x30; the def's FireResistance at +0x28.
// Shape: `for_each(begin, end, mem_fun_ref(&CFirePoint::Reset))` with its result discarded (the frame
// word); the component used straight from the lookup; the state reset in member order with one zero
// register; ParentThing pushed before the GetWorld call.
// Shape: the opinion manager is reached by naming the GetWorld() CALL result (`CWorld* world = GetWorld(); world->POpinionReactionManager->Clear...`):
//   the member read stays in the call expression and lands in ecx (a named manager local goes through eax + mov ecx,eax).
struct CTCBase;
struct CThing;

namespace NFire {
struct CFirePoint {
    unsigned char _data[0x2c];
    void Reset();                                           // 0x00780700
};
}

template<class R, class T> struct mem_fun_ref_t {
    R (T::*m_pmf)();
    explicit mem_fun_ref_t(R (T::*pmf)()) : m_pmf(pmf) { }
    R operator()(T& item) const { return (item.*m_pmf)(); }
};
template<class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pmf)()) { return mem_fun_ref_t<R, T>(pmf); }

template<class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x00781370

template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};

struct CTCSound {
    void StopSound();                                       // 0x0066AEB0
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct COpinionReactionManager {
    void ClearOpinionSource(CThing* source, bool immediately);   // 0x007008C0
};

struct CWorld {
    unsigned char            _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;       // +0x74
};

struct CFlammableDef {
    unsigned char _pad[0x28];
    long FireResistance;                                    // +0x28
};

struct CTCFlammable {
    virtual void v00();
    CThing*        ParentThing;                             // +0x04 (CTCBase)
    bool           ToBeRemoved;                             // +0x08
    bool           IsInGlobalUpdateTCs;                     // +0x09
    CFlammableDef* PDef;                                    // +0x0c CDefPointer<const CFlammableDef>
    CRetailVector<NFire::CFirePoint> FirePoints;            // +0x10
    bool           Burning;                                 // +0x1c
    long           FireDamageAfterFrame;                    // +0x20
    long           FireResistance;                          // +0x24
    long           FireStrength;                            // +0x28
    float          DamageCaused;                            // +0x2c
    bool           DamagePerPoint;                          // +0x30
    bool           HasFirePoint;                            // +0x31
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    void InitialiseFirePoints();                            // 0x007808B0
    void ExtinguishFire();
};

void CTCFlammable::ExtinguishFire()
{
    for_each(FirePoints.begin(), FirePoints.end(), mem_fun_ref(&NFire::CFirePoint::Reset));
    if (ParentThing->HasInterface(16))
        ((CTCSound*)ParentThing->GetInterface(16))->StopSound();
    Burning = false;
    FireDamageAfterFrame = 0;
    FireResistance = PDef->FireResistance;
    FireStrength = 1;
    DamageCaused = 0.0f;
    DamagePerPoint = false;
    InitialiseFirePoints();
    CThing* thing = ParentThing;
    CWorld* world = GetWorld();
    world->POpinionReactionManager->ClearOpinionSource(thing, true);
}