// std::pair<const unsigned __int64, CCombatantInfo>::`scalar deleting destructor'(unsigned int flags) -- destroy the defender info and the attacker's combat-sequence vector, then free when flags & 1
// Leaf: ??_G?$pair@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4AF0, 65 bytes, `ret 4`.
// Label correction: catalog "std_pair_EAnimComponent_CIVCountedPointer_dtor" is propagated; this is the scalar
//   deleting destructor of pair<const unsigned __int64, CCombatantInfo> (the combatant map's value), emitted for the
//   explicit `_Destroy(pair*)`.
// Callees: ~CDefendingCombatantInfo 0x006F48A0, __destroy_aux<CCountedPointer<CCombatSequenceBase>*> 0x006F2300
//   (landed separately; same-TU noinline body), free 0x00BFEA14, operator delete 0x00BFE9BC.
// Layout: CCombatantInfo (Ego_r.pdb; retail 0x70): Parent +0x00, AttackerInfo (0x30; CombatSequences vector +0x24)
//   +0x04, DefenderInfo (0x3c) +0x34; the pair's key is 8 bytes, so the value starts at +0x08.
// Shape: VC7.1 routes an explicit `p->~T()` through `??_G` with flags 0; members die in reverse order.
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // MSVCR71 import thunk 0x00BFEA14
void __cdecl operator delete(void* block);                // MSVCR71 import thunk 0x00BFE9BC

class CThingCreatureBase;
class CCombatSequenceBase;
class CCPPointerInfo;

template<class T> class CCountedPointer {                 // Ego_r.pdb: 8 bytes
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    ~CCountedPointer();
};

class C2DVector {
public:
    float X, Y;
};

namespace std {
struct __false_type { __false_type() {} };

template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }

#pragma auto_inline(off)
// 0x006F2300 (landed separately; same-TU body so the empty tag can share a dead or live argument slot)
template<class ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)

template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template<class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template<class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};
}

class CAttackingCombatantInfo {                           // Ego_r.pdb (retail 0x30: 12-byte STLport vector)
public:
    CThingCreatureBase* Attacked;                         // +0x00
    long AttackerNumber;                                  // +0x04
    long CombatWheelRingIndex;                            // +0x08
    bool PreferredAttackDollySet;                         // +0x0c
    long PreferredAttackDollyTimer;                       // +0x10
    C2DVector PreferredAttackDollyVector;                 // +0x14
    bool UsingCombatWheel;                                // +0x1c
    long CurrentSequence;                                 // +0x20
    std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
};

class CDefendingCombatantInfo {                           // retail 0x3c bytes (12-byte STLport containers)
public:
    unsigned char _body[0x3c];
    ~CDefendingCombatantInfo();                           // 0x006F48A0
};

struct CCombatantInfo {                                    // Ego_r.pdb (retail 0x70)
public:
    CThingCreatureBase* Parent;                           // +0x00
    CAttackingCombatantInfo AttackerInfo;                 // +0x04
    CDefendingCombatantInfo DefenderInfo;                 // +0x34
};

typedef std::pair<const unsigned __int64, CCombatantInfo> SCombatantByUID;   // 0x78 bytes; tree node 0x88

template void std::_Destroy(SCombatantByUID* pointer);