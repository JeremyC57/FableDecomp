// CPlayer::GetPNearestTCVillage() -- the village component of the village thing nearest to the living player character (0 when the character is dead or no village leaf thing is found)
// Grade: DIFFER(196v196) -- argument build scheduling (comparator stores and the ThingSearchTools load around the &village push) and the GetInterface key / out pointer homed in the dead filter word (frame+4) where retail reuses the village local's word (frame+0) | tried (chunk 630): PDB accessors CPlayer::GetWorld / CWorld::GetThingSearchTools, const GetPBestLeafThing (FableWin QBE), C3DVector with user ctors + copy ctor (same residue) | next: lane gap: dead-slot choice between an address-taken out pointer and a dead by-reference temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488620, length 208 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb `?GetPNearestTCVillage@CPlayer@@QAEPAVCTCVillage@@XZ` (the const twin is 0x004886F0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x006A4D00,
// CThingSearchTools::GetPBestLeafThing<binder2nd<const_mem_fun1_ref_t<bool,CThing,ETCInterfaceType> >,
// CThingCompare_Nearest, CThingVillage> 0x0048CB50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
// the bound predicate is &CThing::HasTC 0x004118C0 with TC_VILLAGE (0x22).
// Layout: World +0x1C (its CThingSearchTools at +0x20), PlayerCharacter +0x34; CThing dead bit +0x91 & 1, interface
// bitset +0x20 (type 0x22 = word +0x24, bit 2), interface map +0x44.
// Shape: the search takes the filter, the comparator (a copy of the character position) and the out pointer by
// reference; the result pointer is zeroed first. The village component comes from the unchecked out-parameter
// GetInterface: on the absent arm the never-written out pointer is read back from its frame word, which VC shares
// with the dead village local and the lookup key (`mov eax,[esp+8]`).
enum ETCInterfaceType { TC_VILLAGE = 0x22 };

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CTCBase;
struct CTCVillage;
struct CBaseObject;

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    bool HasTC(ETCInterfaceType type) const;            // 0x004118C0
    const C3DVector& GetPosition() const;               // 0x006A4D00
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingVillage : CThing { };
struct CThingPlayerCreature : CThing { };

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

namespace std {
template <class R, class T, class A> class const_mem_fun1_ref_t {
public:
    explicit const_mem_fun1_ref_t(R (T::*pf)(A) const) : _M_f(pf) { }
    R operator()(const T& r, A x) const { return (r.*_M_f)(x); }
private:
    R (T::*_M_f)(A) const;
};
template <class R, class T, class A> inline const_mem_fun1_ref_t<R, T, A> mem_fun_ref(R (T::*pf)(A) const)
{
    return const_mem_fun1_ref_t<R, T, A>(pf);
}
template <class Op, class V> class binder2nd {
public:
    binder2nd(const Op& x, const V& y) : op(x), value(y) { }
protected:
    Op op;
    V  value;
};
template <class Op, class V> inline binder2nd<Op, V> bind2nd(const Op& op, const V& x)
{
    return binder2nd<Op, V>(op, x);
}
}

struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};

typedef std::binder2nd<std::const_mem_fun1_ref_t<bool, CThing, ETCInterfaceType>, ETCInterfaceType> CHasTCFilter;
struct CThingSearchTools {
    template <class TFilter, class TCompare, class TThing>
    bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** best) const;   // 0x0048CB50 (this instantiation)
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools_ptr;           // +0x20 scoped_ptr<CThingSearchTools>
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools_ptr; }
};

struct CPlayer {
    void*         __vftable;
    unsigned char _pad04[0x1c - 0x04];
    CWorld*       World;                                // +0x1c
    unsigned char _pad20[0x34 - 0x20];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CTCVillage* GetPNearestTCVillage();
private:
    CWorld* GetWorld() { return World; }
};

CTCVillage* CPlayer::GetPNearestTCVillage()
{
    if (IsThingAlive(PlayerCharacter.GetPItem())) {
        CThingVillage* village = 0;
        if (GetWorld()->GetThingSearchTools().GetPBestLeafThing(std::bind2nd(std::mem_fun_ref(&CThing::HasTC), TC_VILLAGE),
                CThingCompare_Nearest(PlayerCharacter.GetPItem()->GetPosition()), &village)) {
            CTCVillage* tc;
            village->GetInterface(&tc, TC_VILLAGE);
            return tc;
        }
    }
    return 0;
}
