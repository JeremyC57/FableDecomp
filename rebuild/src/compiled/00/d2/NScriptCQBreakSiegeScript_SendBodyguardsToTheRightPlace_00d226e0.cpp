// NScript::CQ_BreakSiegeScript::SendBodyguardsToTheRightPlace(CCharString first, CCharString second) -- collect the "BodyGuard" things; with one bodyguard teleport it to the `first` marker, with two teleport them to `first` and `second` (each step bails out when the script thread is terminating)
// Leaf: ?SendBodyguardsToTheRightPlace@CQ_BreakSiegeScript@NScript@@QAEXVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D226E0, 413 bytes, thiscall `ret 8` (two by-value CCharStrings destroyed by the callee).
// Identity (proven): FableWin.pdb ?SendBodyguardsToTheRightPlace@CQ_BreakSiegeScript@NScript@@QAEXVCCharString@@0@Z (Ego_r has only the
//   CV_HauntedHouseScript method of the same signature) -- two
//   CCharString by value (`ret 8`, both destroyed at exit), the "BodyGuard" literal 0x012CB3F0, and its three callers 0x00D21D37 /
//   0x00D21F88 / 0x00D21FE5 sit in CQ_BreakSiegeScript code (landed WatchForWinLoseConditions 0x00D20790 is the same class; the
//   CV_HauntedHouseScript method of the same name lives elsewhere). Catalog label "CScriptBase_InitializeBodyGuardAbility" is wrong.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   ~vector<CScriptThing> 0x008AC970 (the out-of-line copy on the terminating return; the normal exit expands it inline with
//   free 0x00BFEA14), ~CScriptThing 0x004AA840 (the GetThingWithScriptName temporaries), CScriptThing slot 0 (virtual dtor);
//   interface (+0x40) slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 472 EntityTeleportToThing
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Layout: CQ_BreakSiegeScript model of the landed WatchForWinLoseConditions 0x00D20790 (CScriptBase 0x40 bytes, Game +0x40).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public:                            // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                            // slot 0; complete destructor 0x004AA840
    void* PImp_Data;
    void* PImp_Info;
};
namespace std {
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for (; __first != __last; ++__first)
        __first->~_Tp();
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()                                           // 0x008AC970 (CScriptThing)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return _M_start[__n]; }
};
}   // namespace std
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                             // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_BreakSiegeScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void SendBodyguardsToTheRightPlace(CCharString first, CCharString second);
};

void CQ_BreakSiegeScript::SendBodyguardsToTheRightPlace(CCharString first, CCharString second)
{
    std::vector<CScriptThing> guards;
    Game->GetAllThingsWithScriptName("BodyGuard", guards);
    if (guards.size() == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(guards[0], Game->GetThingWithScriptName(first), false);
    }
    if (guards.size() == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(guards[0], Game->GetThingWithScriptName(first), false);
        Game->EntityTeleportToThing(guards[1], Game->GetThingWithScriptName(second), false);
    }
}
}   // namespace NScript