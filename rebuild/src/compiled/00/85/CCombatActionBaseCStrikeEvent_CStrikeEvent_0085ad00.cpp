// CCombatActionBase::CStrikeEvent::~CStrikeEvent() -- the compiler-generated destructor: free the StruckThingList block, destroy PWeapon, then HelperPointName
// Leaf: ??1CStrikeEvent@CCombatActionBase@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AD00, 36 bytes, thiscall, tail `jmp ~CCharString`.
// Identity (proven): unreferenced in retail (the class's `??_G` 0x0085AE90, landed, expands the same three member
//   destructions inline; the linker kept this COMDAT). FableWin.pdb has the non-virtual pair
//   ??_GCStrikeEvent@CCombatActionBase@@QAEPAXI@Z (0x0282D400) then ??1CStrikeEvent@CCombatActionBase@@QAE@XZ (0x0282D440);
//   Ego_r.pdb CCombatActionBase::CStrikeEvent: HelperPointName (CCharString) +0x04, PWeapon (CIntelligentPointer<const
//   CThing>) +0x14, StruckThingList (vector<CThingGameObject*>) +0x1C -- the three members this body destroys, in reverse
//   order. It sits in the CCombatActionBase TU next to the CStrikeEvent ctor 0x0085ADB0. Not polymorphic (QAE, no vtable).
//   The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10
//   (CIntelligentPointer<const CThing> has no destructor of its own in retail: its implicit one is the base's out-of-line
//   call), CCharString::~CCharString 0x0099EAE0.
// Shape: members in reverse order; the landed ??_G 0x0085AE90 model with the real STLport vector. DestroyOne (a destruction
//   by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;                           // +0x00
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingGameObject;

class CCombatActionBase {
public:
    class CStrikeEvent {                                    // Ego_r.pdb, 0x28 bytes in retail
    public:
        CStrikeEvent(CThing* weapon, long strikeEventIndex, const CCharString& helperPointName, long startFrame,
                     bool isWeapon, bool legacy, bool primaryLegacy);   // 0x0085ADB0
        bool                              Legacy;           // +0x00
        bool                              PrimaryLegacy;    // +0x01
        bool                              Weapon;           // +0x02
        CCharString                       HelperPointName;  // +0x04
        long                              StrikeEventIndex; // +0x08
        long                              StartFrame;       // +0x0c
        long                              EndFrame;         // +0x10
        CIntelligentPointer<const CThing> PWeapon;          // +0x14
        std::vector<CThingGameObject*>    StruckThingList;  // +0x1c
    };
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CCombatActionBase::CStrikeEvent* strikeEvent)
{
    strikeEvent->CCombatActionBase::CStrikeEvent::~CStrikeEvent();
}