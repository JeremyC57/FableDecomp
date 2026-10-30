// NScript::CQ_BanditCampBossBattleScript::CCampHostage2::Init() -- the second hostage joins FACTION_NEUTRALS and has opinion reaction 34 disabled
// Leaf: ?Init@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D08720, 70 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012C9080, which the landed
//   NScript::CQ_BanditCampBossBattleScript::CCampHostage2::Alloc 0x00D08670 stores (site 0x00D086A6); Ego_r.pdb has
//   ?Init@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: script interface slots 601 EntitySetInFaction (+0x964), 617 EntitySetOpinionReactionEnabled (+0x9a4);
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("FACTION_NEUTRALS" 0x012C7BC0).
// Layout: Ego_r.pdb CCampHostage2: ParentClass +0x14, PMasterData +0x18. The reaction type 34 has no proven enumerator name.
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
enum EOpinionReactionType { };
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampBossBattleScript {
public:
    class CCampHostage2;
};
class CQ_BanditCampBossBattleScript::CCampHostage2 : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void Init();
};

void CQ_BanditCampBossBattleScript::CCampHostage2::Init()
{
    Game->EntitySetInFaction(Me, "FACTION_NEUTRALS");
    Game->EntitySetOpinionReactionEnabled(Me, (EOpinionReactionType)34, false);
}
}