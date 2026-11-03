// NScript::CV_AssassinAttacksScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_AssassinAttacksScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E39880, 30 bytes, thiscall, `ret 4`; calls 00e398a0=NScript::CV_AssassinAttacksScript::~CV_AssassinAttacksScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012E35B0 (no RTTI), the vtable the landed constructor 0x00E376D0 stores last
//   (??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, FableWin.pdb); Ego_r.pdb ??_ECV_AssassinAttacksScript@NScript@@UAEPAXI@Z; FableWin.pdb ??_ECV_AssassinAttacksScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??1CV_AssassinAttacksScript@NScript@@UAE@XZ; FableWin.pdb ??1CV_AssassinAttacksScript@NScript@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012e35b0::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00E376D0 (rebuild/src/compiled/00/e3/NScriptCVAssassinAttacksScript_CVAssassinAttacksScript_00e376d0.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added. The constructor body is not part of this graded body.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData; }
class CGameScriptInterfaceBase;
class CPersistContext;
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"
namespace NScript {
enum AA_LocationIdentifier { AA_LOCATION_NONE };
class CV_AssassinAttacksScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    class RegionData {
    public:
        CCharString name;                               // +0x00
        long count;                                     // +0x04
        long max;                                       // +0x08
        bool assassinActive;                            // +0x0c
    };
    CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CV_AssassinAttacksScript();
    AA_LocationIdentifier RegionIndex;                  // +0x48
    long AssassinObjectCount;                           // +0x4c
    bool Activated;                                     // +0x50
    CCharString RegionOfMostRecentKill;                 // +0x54
    RegionData RegionInfo[5];                           // +0x58
    std::vector<long> RegionKillCounts;                 // +0xa8
    bool QuestComplete;                                 // +0xb4
    bool RewardsGiven;                                  // +0xb5
    bool GivenQuestCard;                                // +0xb6
};
CV_AssassinAttacksScript::CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}