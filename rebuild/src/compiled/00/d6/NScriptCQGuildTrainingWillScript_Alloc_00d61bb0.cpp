// NScript::CQ_GuildTrainingWillScript::Alloc(CScriptDataBase* data,CGameScriptInterfaceBase& game) -- Allocate the original typed script object.
// Leaf: ?Alloc@CQ_GuildTrainingWillScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Grade: RELOCATION_MATCH (wave-2 sec6: length from the retail bytes, BOUNDS_FIX/oracle_extra; moved onto the shared
//   retail NScript headers CLevelScriptBase.h/CScriptThing.h/CCountedPointer.h, bytes unchanged)
// Identity: own script-info registration00cd62fe literalQ_GuildTrainingWill -> factory -> constructor00d5dc50/vtable012d1038; exact original PDB signature.
// Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation108. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.
// Headers: retail NScript models (engine/CLevelScriptBase.h -> CScriptBase 0x40, Game +0x40, MasterScriptData +0x44;
//   engine/CScriptThing.h, 0xc); STLport 4.6 vector modelled locally (no shared STL header).
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
    CQ_GuildTrainingWillScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CQ_GuildTrainingWillScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingWillScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_GuildTrainingWillScript) == 108 ? 1 : -1];