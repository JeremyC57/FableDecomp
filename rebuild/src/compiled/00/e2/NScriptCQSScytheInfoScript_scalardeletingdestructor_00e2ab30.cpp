// NScript::CQS_ScytheInfoScript::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CQS_ScytheInfoScript expanded (the two STLport map members destroyed out of line, then ~CScriptBase), then free the object when bit 0 is set
// Leaf: ??_GCQS_ScytheInfoScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2AB30, 48 bytes, thiscall, `ret 4`: push esi ; mov esi,ecx ; lea ecx,[esi+58h] ; call 0x00CDBFB0 ;
//   lea ecx,[esi+4Ch] ; call 0x009AC310 ; mov ecx,esi ; call 0x00CBD510 ; test [esp+8],1 ; je ; push esi ; call operator delete
//   0x00BFE9BC ; add esp,4 ; mov eax,esi ; pop esi.
// Identity (two sources): slot 0 of the script vtable 0x012E22C8, the vtable the landed constructor 0x00E299F0
//   (??0CQS_ScytheInfoScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) stores, whose slots 1..3 are the landed
//   ?RegisterMain@CQS_ScytheInfoScript@NScript@@UAEXXZ 0x00E29A80, ?Main@... 0x00E29B20 and ?Init@... 0x00E29A70 (slot 4 is CScriptBase's
//   OnPersist 0x00CBD4E0); Ego_r.pdb ??_GCQS_ScytheInfoScript@NScript@@UAEPAXI@Z (0x00E77610, right after ??1 0x00E77500).
// Callees: std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map 0x00CDBFB0 (cs_chars), std::map<CCharString,CCharString>
//   ::~map 0x009AC310 (cs_args), CScriptBase::~CScriptBase 0x00CBD510 (through the implicit ~CLevelScriptBase<T>), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CQS_ScytheInfoScript (members MissionSucceeded, cs_args, cs_chars; 128 bytes on the donor base) is native 0x64 (the Alloc
//   0x00E2AB00 news 0x64 bytes): CLevelScriptBase<T> 0x48, MissionSucceeded +0x48, cs_args +0x4C, cs_chars +0x58 (STLport maps, 12 bytes).
//   No engine/ header for CQS_ScytheInfoScript: the class is modelled here in the landed siblings' shape (CQS_GuardianSisterInfoScript ??_G
//   0x00E267C0: a CLevelScriptBase<CQ_SunnyvaleMasterData> level script; the entity scripts of this class cast PMasterData to
//   CQ_SunnyvaleMasterData*), its base from the shared engine/CLevelScriptBase.h.
// Fidelity: scaffolding, not retail. The ??_G is compiler-generated with the vtable; this row reaches it by constructing the class (inline
//   PDB constructor, retail out of line at 0x00E299F0, owned by that landed row) from an anonymous-namespace helper (nf86 / nf163 form), so
//   VC7.1 emits the vtable and its ??_G as pick-any COMDATs. The map constructors are STLport's header inlines (fable_map.h), so the
//   constructor copy VC7.1 emits equals 0x00E299F0's 113 bytes (it grades RELOCATION_MATCH there; one LNK2005 against that row's strong
//   definition, identical bytes: debt until 0x00E299F0 takes this pick-any form); the map destructors are declared only
//   (FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY / FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY), so no ~map / ~_Rb_tree / clear / _M_erase copies.
// Master data: CQ_SunnyvaleMasterData (Ego_r.pdb types the nested CScytheMarker / CScytheNearOracle PMasterData +0x18 as
//   CQ_SunnyvaleMasterData*; the landed entity rows cast to it).
// Name fix (NF209, 2026-11-20, q4860): was the free __fastcall stand-in CThingSwitch_CompositeScalarDeletingDestructor calling AutoTiny
//   stand-ins (catalog NScript::CQS_ScytheInfoScript::`scalar deleting destructor').
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY    // ~map declared only: retail calls the map destructors 0x00CDBFB0 / 0x009AC310 out of line
#define FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY   // ~_Rb_tree declared only (no clear / _M_erase copies)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_ScytheInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_ScytheInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)       // 0x00E299F0 (Ego_r / FableWin)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
    {
    }
    virtual void RegisterMain();                                                       // 0x00E29A80
    virtual void Main();                                                               // 0x00E29B20
    virtual void Init();                                                               // 0x00E29A70
    bool MissionSucceeded;                                                             // +0x48
    std::map<CCharString,CCharString> cs_args;                                         // +0x4C
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cs_chars;         // +0x58
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_ScytheInfoScript)==0x64);
namespace {
// Internal to this TU and never called: constructing the class makes this object emit its vtable with the ??_G (the graded leaf).
void ReferenceScytheInfoScriptVtable(void* where, CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    new (where) NScript::CQS_ScytheInfoScript(data, game);
}
}
