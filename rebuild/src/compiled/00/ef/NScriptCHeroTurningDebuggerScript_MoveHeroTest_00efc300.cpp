// NScript::CHeroTurningDebuggerScript::MoveHeroTest() -- debug thread: until the script thread terminates, take the hero under scripted control, move him to the MarkerToFace marker, wait 30 seconds, release him and wait a frame
// Leaf: ?MoveHeroTest@CHeroTurningDebuggerScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFC300, length 315 bytes, thiscall, plain `ret`.
// Name: the row is referenced once, at 0x00EFC2BA inside the landed NScript::CHeroTurningDebuggerScript::Main 0x00EFC260,
//   which spawns it as the "MoveHeroTest" thread (CSpawnedFunc member pointer at +0x34); Ego_r.pdb has
//   ?MoveHeroTest@CHeroTurningDebuggerScript@NScript@@QAEXXZ. (Catalog label Script_MarkerToFace_Thread is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseObject::CBaseObject() 0x0099A380 /
//   CBaseObject::~CBaseObject() 0x0099A430 (the scripted-thing handle's root), CScriptGameResourceObjectScriptedThingBase::
//   MoveToPosition 0x007E72F0 (virtual slot 4 of the handle vtable 0x0127094C, called directly on the local),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC; through the script interface (Game, +0x40): NewScriptFrame (slot 7), StartScriptingEntity (slot 8),
//   GetHero (slot 70), GetThingWithScriptName (slot 72, a CScriptThing by value), Pause (slot 376); CScriptThing::GetPos
//   (slot 6, virtual on the temporary).
// Shape: a generated-script `while (!IsActiveThreadTerminating())` loop (rotated: entry test, bottom test). The handle
//   is a block-scoped local destroyed before NewScriptFrame: its implicit destructor releases the counted PImp inline
//   (Data = 0; Info = 0), then the inline ~CScriptGameResourceObjectBase stores the base vtable 0x0126008C and calls
//   ~CBaseObject. The marker thing is a by-value temporary destroyed at the end of the full expression (inline
//   ~CScriptThing: vtable 0x01238C8C, release, ~CBaseClass). Pause(30.0f) pushes 0x41F00000.
// Layout: engine/CBaseObject.h is flattened (its vptr is a data member, so it cannot be a polymorphic base of the
//   handle classes); CBaseObject is modelled locally with the header's members (IntelligentPointer +0x04, 8 bytes).
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables), CScriptThing / handle classes after the landed CQ_GuildTrainingMeleeScript::Main 0x00D55E90 model.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);            // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
};
void operator delete(void* p);                                  // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}

class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptEntityMoveType { SCRIPT_MOVE_DEFAULT };

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"

class CScriptDataBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CDanyalTestScriptsData;
// Master data (wave 8): RegisterScripts 0x00CD52D0 pairs CHeroTurningDebuggerScript's Alloc with the CDanyalTestScriptsData 0x00CD4B80 Alloc (wave 7 HEADERS.md table), so T is CDanyalTestScriptsData.
class CHeroTurningDebuggerScript : public CLevelScriptBase<CDanyalTestScriptsData> {
public:
    void MoveHeroTest();
};
} // namespace NScript

void NScript::CHeroTurningDebuggerScript::MoveHeroTest()
{
    while (!IsActiveThreadTerminating()) {
        {
            CScriptGameResourceObjectScriptedThingBase hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
            hero.MoveToPosition(Game->GetThingWithScriptName("MarkerToFace").GetPos(), 0.0f, (EScriptEntityMoveType)0, false, true);
            Game->Pause(30.0f);
        }
        Game->NewScriptFrame();
    }
}