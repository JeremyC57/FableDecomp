// NScript::CV_AssassinAttacksScript::~CV_AssassinAttacksScript() -- implicit village-script destructor: free the RegionKillCounts block, the five RegionInfo names (count-down loop), RegionOfMostRecentKill, then ~CScriptBase
// Leaf: ??1CV_AssassinAttacksScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E398A0, 66 bytes (+ int3 padding), thiscall; tail `jmp ~CScriptBase` 0x00CBD510.
// Identity (proven, two sources): the only caller 0x00E39883 is the scalar deleting destructor 0x00E39880, slot 0 of the RTTI-less vtable
//   0x012E35B0; that vtable is stored only by the ctor 0x00E376D0, whose only caller is the Alloc 0x00E39850 (`push 0xB8` ; operator
//   new ; ctor), stored by the script-registration routine next to the save tag "S_VAA" (0x012F777C); refs/script_recovery/
//   conversion_queue.tsv lists that tag and allocator as the village script V_AssassinAttacks, i.e. NScript::CV_AssassinAttacksScript (FableWin.pdb
//   ??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z and ??1CV_AssassinAttacksScript@NScript@@UAE@XZ). The body stores
//   no vtable of its own: the compiler-generated destructor. The catalog label is propagated and wrong (NAME_FIXES.tsv).
// Callees: free 0x00BFEA14 (~vector<long>: the trivial _Destroy is empty, ~_Vector_base frees a non-null block),
//   ??1CCharString@@QAE@XZ 0x0099EAE0 (RegionInfo[5].name by an inline count-down loop with a 16-byte stride, then
//   RegionOfMostRecentKill), ??1CScriptBase@@UAE@XZ 0x00CBD510 (tail).
// Layout: Ego_r.pdb members at Ego_r offset - 0x14 (retail CScriptBase 0x40 vs Ego_r 0x54; CLevelScriptBase<T> Game +0x40 /
//   MasterScriptData +0x44) (RegionIndex +0x48 .. RegionInfo[5] +0x58 (16-byte RegionData), RegionKillCounts STLport vector<long> +0xA8 (12
//   bytes; Ego_r's 16-byte Dinkumware vector puts the bools 4 higher), the three bools +0xB4; sizeof 0xB8 = the Alloc's `push 0B8h`). CLevelScriptBase<T> comes from engine/CLevelScriptBase.h with the retail opt-in
//   FABLE_RETAIL_CLEVELSCRIPTBASE (hdr/ overlay, chunk 713): retail's ~CLevelScriptBase<T> is compiler-generated, so the implicit
//   derived dtor tail-jumps ~CScriptBase; with the header's default out-of-line declaration it would jump to ??1?$CLevelScriptBase.
//   T = NScript::CQ_SunnyvaleMasterData as in the landed village-script rows (0x00E376D0, 0x00ED7640); T changes no byte or symbol
//   here and is not separately proven for this script.
// TU: the PDB ctor ??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined (emission only, body not
//   reconstructed: the landed row is NScript::CV_AssassinAttacksScript 0x00E376D0), which emits the vtable, the scalar deleting dtor and this implicit dtor, as retail's TU does.
extern "C" void __cdecl free(void* block);    // 0x00BFEA14 (STLport allocator<long>::deallocate)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "fable_string.h"                     // CCharString (~CCharString 0x0099EAE0)
#define FABLE_RETAIL_CLEVELSCRIPTBASE         // retail's implicit ~CLevelScriptBase<T> (hdr/ overlay, chunk 713)
#include "engine/CLevelScriptBase.h"          // CLevelScriptBase<T> : CScriptBase (Game +0x40, MasterScriptData +0x44)
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<long> { typedef __true_type has_trivial_destructor; };
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
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
enum AA_LocationIdentifier { AA_LOCATION_NONE };
class CV_AssassinAttacksScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {   // vtable 0x012E35B0 (no RTTI)
public:
    class RegionData {                              // Ego_r.pdb 0x10
    public:
        CCharString name;                           // +0x00
        long        count;                          // +0x04
        long        max;                            // +0x08
        bool        assassinActive;                 // +0x0c
    };
    CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    AA_LocationIdentifier RegionIndex;              // +0x48
    long                  AssassinObjectCount;      // +0x4c
    bool                  Activated;                // +0x50
    CCharString           RegionOfMostRecentKill;   // +0x54
    RegionData            RegionInfo[5];            // +0x58
    std::vector<long>     RegionKillCounts;         // +0xa8 (STLport, 12 bytes)
    bool                  QuestComplete;            // +0xb4
    bool                  RewardsGiven;             // +0xb5
    bool                  GivenQuestCard;           // +0xb6
};
FABLE_STATIC_ASSERT(sizeof(CV_AssassinAttacksScript) == 0xb8);

CV_AssassinAttacksScript::CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E376D0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}