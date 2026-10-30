// CTCEffectOnDie::OnDie() -- unless paused, play the death sound criteria through the parent's sound component and create the death effect thing at the parent's position
// Grade: DIFFER(112v112) -- wave 2 sec3: callee respelled to the bound PDB CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) 0x0066BD00, PDB CThing::GetTC reference getter with a same-TU noinline LowerBound 0x0040F020 body; residue unchanged (the component load into ecx before the pushes). Tried: same-TU LowerBound body, GetTC& + static_cast, out-param GetInterface<T> (loads early but into eax: 114v112, probe/007e0d00_v4.cpp). Next: an inline PDB-less wrapper is the only form left
// Leaf: OnDie
//
// Retail 0x007E0D00, 112 bytes (plain `ret`; vtable 0x0126FEB4 slot 13, Ego_r.pdb
// `?OnDie@CTCEffectOnDie@@UAEXXZ`; the catalog "CTCEffectOnDie::vfunc_13" is the slot name).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// the sound component, interface 16), the sound component's play method @ 0x0066BD00 (here with the
// criteria string, 1, 0, 0, -1), CThing::GetPosition() const @ 0x004C73D0, the global Create(long,
// const C3DVector&, bool) @ 0x006E0880 (`__fastcall`, the bool pushed first; result unused).
// Layout = Ego_r.pdb CTCEffectOnDie (0x24): Effect +0x10, DeathSoundCriteria +0x1c, Paused +0x21.
// Shape: the component is a named local taken right after the lookup (`mov ecx,[eax+4]` before the
// pushes; used straight in the call it is loaded after them); the effect is created only when its index is non-zero.
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    CThing*     ParentThing;                                // +0x04
    bool        ToBeRemoved;                                // +0x08
    bool        IsInGlobalUpdateTCs;                        // +0x09
};
class C3DVector;

class CCharString { public:
    void* PStringData;
};

class CTCSoundPlayer : public CTCBase                       // the thing's interface-16 component
{
public:
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};

enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    __declspec(noinline) value_type* LowerBound(const K& key)       // 0x0040F020 (same-TU body: the caller sees its mod-ref)
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;
    bool Dirty;
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase& GetTC(ETCInterfaceType type)        // Ego_r ?GetTC@CThing@@QAEAAVCTCBase@@W4ETCInterfaceType@@@Z (inline)
    {
        CInterfaceMap* map = &m_interfaces;
        const ETCInterfaceType key = type;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        CInterfaceMap::value_type* end = map->_M_finish;
        return *(it == end || type < it->first ? end : it)->second;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x006E0880

class CTCEffectOnDie : public CTCBase
{
public:
    bool        StartedNonRecoverableFadeout;               // +0x0c
    long        Effect;                                     // +0x10
    long        Timer;                                      // +0x14
    long        FadeOutTimer;                               // +0x18
    CCharString DeathSoundCriteria;                         // +0x1c
    bool        Pausable;                                   // +0x20
    bool        Paused;                                     // +0x21
    virtual void OnDie();
};

void CTCEffectOnDie::OnDie()
{
    if (!Paused)
    {
        if (ParentThing->HasInterface(16))
        {
            static_cast<CTCSoundPlayer&>(ParentThing->GetTC((ETCInterfaceType)16)).PlayAttachedCriteria(DeathSoundCriteria, true, false, false, -1);
        }
        if (Effect != 0)
            Create(Effect, ParentThing->GetPosition(), false);
    }
}
