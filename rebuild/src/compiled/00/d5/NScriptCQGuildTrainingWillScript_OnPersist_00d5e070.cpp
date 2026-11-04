// NScript::CQ_GuildTrainingWillScript::OnPersist(CPersistContext& context) -- saves/loads TestFinished and BanditsDefeated by name (default false)
// Leaf: ?OnPersist@CQ_GuildTrainingWillScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D5E070, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012d1038 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d5dd40 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQ_GuildTrainingWillScript; PDB ?OnPersist@CQ_GuildTrainingWillScript@NScript@@; refs=1; was _unattributed::PersistTransfer2_00d5e070.
//   ?OnPersist@CQ_GuildTrainingWillScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d5e070::PersistTransfer2_00d5e070(CPersistContext* context) -- persists the "TestFinished" and "BanditsDefeated" byte flags at +0x5c/+0x5d Retail: Fable.exe (TLC, VC7.1) function @ 0x00D5E070, oracle le... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00D5DC50 (rebuild/src/compiled/00/d5/NScriptCQGuildTrainingWillScript_CQGuildTrainingWillScript_00d5dc50.cpp); its bool TestFinished, bool BanditsDefeated. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
namespace std {
template <class T> class allocator { };
template <class V, class T, class A> class _STLP_alloc_proxy : public A { public: V _M_data; _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { } };
template <class T, class A> class _Vector_base { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { } };
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> { public: vector() : _Vector_base<T, A>(A()) { } };
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWillScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long TargetsHit;                                   // +0x48
    long TutorialState;                                // +0x4c
    long TotalTrainingDummiesCounter;                  // +0x50
    long GenericTutorialCounter;                       // +0x54
    long HeroHit;                                      // +0x58
    bool TestFinished;                                 // +0x5c
    bool BanditsDefeated;                              // +0x5d
    std::vector<CScriptThing> DummyWizardsVector;      // +0x60 (108 bytes total)
    CQ_GuildTrainingWillScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CQ_GuildTrainingWillScript::CQ_GuildTrainingWillScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_GuildTrainingWillScript) == 108 ? 1 : -1];

void NScript::CQ_GuildTrainingWillScript::OnPersist(CPersistContext& context)
{
    context.Transfer("TestFinished", TestFinished, false);
    context.Transfer("BanditsDefeated", BanditsDefeated, false);
}
