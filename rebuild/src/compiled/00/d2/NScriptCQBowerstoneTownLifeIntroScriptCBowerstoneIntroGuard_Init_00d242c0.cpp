// NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuard::Init() -- the guard carries information-less dialogue and cannot be killed
// Leaf: ?Init@CBowerstoneIntroGuard@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D242C0, 44 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CB5C4, which the landed
//   NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuard::Alloc 0x00D24210 stores (site 0x00D24246); Ego_r.pdb has
//   ?Init@CBowerstoneIntroGuard@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: script interface slots 360 SetThingHasInformation (+0x5a0) and 517 EntitySetAsKillable(const CScriptThing&, bool, bool) (+0x814).
// Layout: Ego_r.pdb CBowerstoneIntroGuard: ParentClass +0x14, PMasterData +0x18.
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
class CQ_BowerstoneTownLifeIntroScript {
public:
    class CBowerstoneIntroGuard;
};
class CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuard : public CEntityScriptBase {
public:
    CQ_BowerstoneTownLifeIntroScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void Init();
};

void CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuard::Init()
{
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetAsKillable(Me, false, true);
}
}