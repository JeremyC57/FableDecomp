// CThingCreatureBase::GetAppropriateCombatAnimationFromList(const vector<CAnimationDef*>& anims) -- with a combat-animation component (interface 0x30) let it choose; otherwise a world-random pick among the animations whose combat stance matches the creature's (stance 2 accepts all), falling back to any of the list; -1 for an empty list
// Leaf: GetAppropriateCombatAnimationFromList
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CLongVector::InsertGrow(..., const SScalarTag&, unsigned long, bool).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const copy), the component's selector @ 0x007345D0,
// the animation's info getter @ 0x00669CB0 (out pointer), vector<long> grow @ 0x00440990, CThing::GetWorld @ 0x004C7AB0,
// CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890 (__fastcall), free @ 0x00BFEA14.
// Layout: retail CThing interface bitset +0x20 (0x30 = word +0x24 bit 16), CVectorMap +0x44; the creature's combat stance
// +0x138 (descriptive); the animation info's stance +0x08.
// Shape: the component lookup is the const out-parameter __forceinline GetInterface in its own scope (its key shares the frame
// word with the loop index); the vector<long> grow template gets a same-TU auto_inline(off) body that does not leak the vector
// (so `last` stays in esi), push_back keeps `last` in a local; the info out-pointer lands in the dead `anims` argument slot by
// itself; the range helper is the ternary `range ? seed % range : 0` (the if/return form puts the zero arm first).
#include <new>
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
long __fastcall GFROR13(unsigned long);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return range ? seed % range : 0;
}

template <class T> __forceinline void Construct(T* p, const T& value) { new (p) T(value); }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* first;
    _Tp* last;
    _Tp* end;
    vector() : first(0), last(0), end(0) { }
    ~vector() { if (first) free(first); }
    bool empty() const { return first == last; }
    _Tp size() const { return last - first; }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990
public:
    void push_back(const _Tp& value)
    {
        _Tp* finish = last;
        if (finish != end) {
            Construct(finish, value);
            last = finish + 1;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
};
}


void* GrowLongVector_00440990(unsigned long count);
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool noTail)
{
    long size = last - first;
    long* p = (long*)GrowLongVector_00440990((size + count) * 2);
    for (long i = 0; i < size; ++i)
        p[i] = first[i];
    p[size] = value;
    if (first)
        free(first);
    first = p;
    last = p + size + count;
    end = p + (size + count) * 2;
}
#pragma auto_inline(on)

struct SCombatAnimInfo {
    unsigned char _pad00[8];
    long Stance;                                // +0x08
};
struct CAnimationDef {
    void GetCombatInfo(const SCombatAnimInfo** out) const;   // 0x00669CB0
};
struct CAnimationDefVector {
    CAnimationDef** first;
    CAnimationDef** last;
    CAnimationDef** end;
    bool empty() const { return first == last; }
    long size() const { return last - first; }
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCCombatAnimations {
    long SelectAnimation(const CAnimationDefVector& anims);   // 0x007345D0
};
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    CWorld* GetWorld();                         // 0x004C7AB0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad4c[0x138 - 0x4c];
    long CombatStance;                          // +0x138
    long GetAppropriateCombatAnimationFromList(const CAnimationDefVector& anims);
};

long CThingCreatureBase::GetAppropriateCombatAnimationFromList(const CAnimationDefVector& anims)
{
    {
        CTCCombatAnimations* combat;
        if (GetInterface(&combat, 0x30))
            return combat->SelectAnimation(anims);
    }
    std::vector<long> indices;
    long index = 0;
    for (CAnimationDef** it = anims.first; it != anims.last; ++it, ++index) {
        const SCombatAnimInfo* info;
        (*it)->GetCombatInfo(&info);
        if (info->Stance == CombatStance || CombatStance == 2)
            indices.push_back(index);
    }
    if (!indices.empty()) {
        long count = indices.size();
        unsigned long* seed = GetWorld()->GetPRandomSeed();
        return WorldRandomRange(*seed, count);
    }
    if (!anims.empty()) {
        long count = anims.size();
        unsigned long* seed = GetWorld()->GetPRandomSeed();
        return WorldRandomRange(*seed, count);
    }
    return -1;
}