// NScript::CQ_HangingTreeEvilScript::WatchForHeroNearExit() -- Hanging Tree (evil) watcher thread: collect every REGION_EXIT_POINT thing, then each frame check whether the hero has come within 8 units of one; the first time he does, the bandit tells him not to leave (a one-line conversation), and the thread ends
// Leaf: ?WatchForHeroNearExit@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6CA90, 555 bytes, thiscall, plain `ret`.
// Identity (proven): the CSpawnedFunc<CQ_HangingTreeEvilScript> (vtable 0x012D1FE8) built in the landed
//   NScript::CQ_HangingTreeEvilScript::SpawnParty 0x00D6C4A0 stores this address as its member function (site 0x00D6C6F5, the
//   row's only reference) right after its "WatchForHeroNearExit" name literal 0x012D2388; Ego_r.pdb
//   ?WatchForHeroNearExit@CQ_HangingTreeEvilScript@NScript@@QAEXXZ (already bound so). Catalog label sub_00d6ca90.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) 0x00CBE2FF (__fastcall), free 0x00BFEA14 (the
//   local vector's storage); each element's virtual destructor (CScriptThing slot 0, `push 0`); CGameScriptInterfaceBase slots
//   7 NewScriptFrame, 70 GetHero, 80 GetAllThingsWithDefName, 364 AddNewConversation, 366 AddLineToConversation.
// Literals: "REGION_EXIT_POINT" 0x012CA510, "TEXT_QST_029_BANDIT_DONT_EXIT" 0x012D23B8; 8.0f (immediate).
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CScriptThing, CLevelScriptBase, CGameScriptInterfaceBase); the thread
//   touches no members of its own class.
// Shape: the local std::vector<CScriptThing> (STLport 4.6, 12 bytes) is destroyed inline at each of the five exits (the
//   _Destroy loop of virtual destructor calls, then free of the block); the outer wait loop is a do-while on the local flag
//   with a termination check before every statement (the generated-script form); both string arguments convert implicitly
//   (the temporary's address is re-taken with `lea` for the push).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
}   // namespace std

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void WatchForHeroNearExit();
};

void CQ_HangingTreeEvilScript::WatchForHeroNearExit()
{
    bool warned = false;
    std::vector<CScriptThing> exitPoints;
    Game->GetAllThingsWithDefName("REGION_EXIT_POINT", exitPoints);
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        for (unsigned long i = 0; i < exitPoints.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), exitPoints[i], 8.0f)) {
                long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_DONT_EXIT", false, Game->GetHero(), Game->GetHero());
                warned = true;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    } while (!warned);
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript