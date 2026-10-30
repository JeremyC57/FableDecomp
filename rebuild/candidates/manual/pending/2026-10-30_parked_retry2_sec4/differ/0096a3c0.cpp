// CAIStateGroup_WhisperPotionCombat::GetValidJumpPosition(C3DVector& pos) -- pick a random valid landing spot near the target: gather the valid positions within 10.5 of it, drop those within 9.5 of the target, those not within 1.5 of its height and those within 3 of a live trap, then try random candidates until the whisper can navigate to one (copied to pos); returns whether one was found
// Grade: DIFFER(820v872) nd=162 (was 823v872 nd=200) -- lever: the same-TU LowerBound stand-in (GOTCHAS chunk 240 idiom); tried: the IsDead accessor on top (162), the navigation lookup inside the try loop (812v872 nd=176); remaining: retail is 52 bytes longer -- this in ebp, the navigation pointer in a frame word shared with the lookup key and the trap index and reloaded every try, start/finish in ebx/esi through the random-try loop
// Leaf: ?GetValidJumpPosition@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096A3C0, 880 bytes (incl. int3 padding), thiscall `ret 4`. FableWin.pdb
//   `?GetValidJumpPosition@CAIStateGroup_WhisperPotionCombat@@IAE_NAAVC3DVector@@@Z`; callers 0x0096AC05, 0x0096B229,
//   0x0096B50B. The catalog's "FindValidCombatPositions" is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CAIStateGroupBase::GetPWorld 0x008FCFC0, the world's slot 12
//   GetWorldMap, NCreatureHiding::GetValidPositions 0x0074A5D0, CThing::GetPosition 0x004C73D0, STLport
//   remove_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> > 0x008AC060, __find_if 0x0096B820 and
//   remove_copy_if 0x0096BA30 (the height-band predicate's instantiations, this TU), __copy<C3DVector*> 0x0074CA20,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 7 = CTCCreatureNavigation), CWorld::GetPRandomSeed
//   0x0049D810, GFROR13 0x00497890, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620, free 0x00BFEA14.
// Layout: as the landed DisappearJump 0x0096AF60: PTarget +0x14, TrapThings (vector of 8-byte intelligent pointers)
//   +0x1C; CWorld's thing-search tools +0x20.
// Shape: a /Gr TU (the STLport helpers it instantiates are __fastcall), the same idiom as the landed
//   CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition 0x008C1600: same-TU noinline __copy and
//   __find_if bodies home the empty iterator tags in the live `pos` slot; the near-point predicates are named locals
//   (only Z goes through the frame); the height-band remove_if is inline, the near-point one is the out-of-line
//   0x008AC060; erase(pos) inlines its element copy; the trap count is read once.
#include <stdlib.h>
#include <math.h>
#pragma intrinsic(fabs)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                        const random_access_iterator_tag&, _Distance*)   // 0x0074CA20
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                                 const random_access_iterator_tag&)      // 0x0096B820
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred);   // 0x0096BA30

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}
template <class _ForwardIter, class _Predicate>
inline _ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    __first = find_if(__first, __last, __pred);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy_if(++__next, __last, __first, __pred);
    }
}
} // namespace std

struct CVectorFilter_FurtherFromPointThan {             // unary_negate<...> of it is the 16-byte remove_if predicate
    float     Radius;                                   // +0x00
    C3DVector CentrePos;                                // +0x04
    CVectorFilter_FurtherFromPointThan(float radius, const C3DVector& centre) : Radius(radius), CentrePos(centre) { }
};
C3DVector* RemoveNearPoint(C3DVector* first, C3DVector* last, CVectorFilter_FurtherFromPointThan pred);   // 0x008AC060

struct CVectorFilter_OutsideHeightBand {                // 8-byte predicate: true unless |z - Height| < Tolerance
    float Height;                                       // +0x00
    float Tolerance;                                    // +0x04
    CVectorFilter_OutsideHeightBand(float height, float tolerance) : Height(height), Tolerance(tolerance) { }
    bool operator()(const C3DVector& p) const { return !(fabs(p.Z - Height) < Tolerance); }
};

struct CPositionVector {                                // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    long size() const { return _M_finish - _M_start; }
    bool empty() const { return begin() == end(); }
    C3DVector& operator[](long i) { return *(begin() + i); }
    C3DVector* erase(C3DVector* first, C3DVector* last)
    {
        C3DVector* newFinish = std::__copy(last, _M_finish, first, std::random_access_iterator_tag(), (long*)0);
        _M_finish = newFinish;
        return first;
    }
    C3DVector* erase(C3DVector* pos)
    {
        if (pos + 1 != _M_finish) {
            C3DVector* src = pos + 1;
            C3DVector* dst = pos;
            for (long n = _M_finish - src; n > 0; --n) {
                *dst = *src;
                ++src;
                ++dst;
            }
        }
        --_M_finish;
        return pos;
    }
};

struct CWorldMap;
struct CThingSearchTools;
struct CTCShapeManager;
void GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                       const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
unsigned long GFROR13(unsigned long value);             // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
#define VF(n) virtual void v##n();
struct CWorld {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11)
    virtual const CWorldMap& GetWorldMap();             // slot 12 (+0x30)
    unsigned char      _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;               // +0x20
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCCreatureNavigation {                          // interface 7
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerVector {                      // STLport vector<CIntelligentPointer<CThing> >
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CBaseIntelligentPointer& operator[](long i) { return *(_M_start + i); }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    long ScriptAIPriority;                              // +0x08
    void* Name_PStringData;                             // +0x0c
    unsigned char Terminated;                           // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() const;                          // 0x008FCFC0
};
struct CAIStateGroup_WhisperPotionCombat : CAIStateGroupBase {
    CBaseIntelligentPointer   PTarget;                  // +0x14
    CIntelligentPointerVector TrapThings;               // +0x1c
    bool GetValidJumpPosition(C3DVector& pos);
};

bool CAIStateGroup_WhisperPotionCombat::GetValidJumpPosition(C3DVector& pos)
{
    bool found = false;
    if (IS_THING_ALIVE(PTarget)) {
        CPositionVector positions;
        CWorld* world = GetPWorld();
        const CThingSearchTools& search = *GetPWorld()->PThingSearchTools;
        GetValidPositions(positions, world->GetWorldMap(), search, PTarget.GetPItem()->GetPosition(), 10.5f, false, 0);
        if (positions.empty())
            return false;
        C3DVector* last = positions.end();
        CVectorFilter_FurtherFromPointThan nearTarget(9.5f, PTarget.GetPItem()->GetPosition());
        positions.erase(RemoveNearPoint(positions.begin(), positions.end(), nearTarget), last);
        CVectorFilter_OutsideHeightBand outsideHeight(PTarget.GetPItem()->GetPosition().Z, 1.5f);
        positions.erase(std::remove_if(positions.begin(), positions.end(), outsideHeight), positions.end());
        long trapCount = TrapThings.size();
        for (long i = 0; i < trapCount; ++i) {
            if (IS_THING_ALIVE(TrapThings[i])) {
                CVectorFilter_FurtherFromPointThan nearTrap(3.0f, TrapThings[i].GetPItem()->GetPosition());
                positions.erase(RemoveNearPoint(positions.begin(), positions.end(), nearTrap), positions.end());
            }
        }
        CTCCreatureNavigation* navigation;
        PBrain->PCreature->GetInterface(&navigation, 7);
        while (!found && positions.size() != 0) {
            unsigned long count = positions.size();
            unsigned long random = GFRandom(GetPWorld()->GetPRandomSeed());
            unsigned long index = count ? random % count : 0;
            if (navigation->IsAbleToNavigateFromPosToPos(PBrain->PCreature->GetPosition(), positions[index], 0.0f)) {
                pos = positions[index];
                found = true;
            } else
                positions.erase(positions.begin() + index);
        }
    }
    return found;
}
