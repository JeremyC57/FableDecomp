// CFrontendGameComponent::~CFrontendGameComponent() -- destroy the debug front end's members (splash texture, world/save/profile name lists and their selections, main font, temporary graphics) and the game-component base
// Leaf: ??1CFrontendGameComponent (Ego_r.pdb ??1CFrontendGameComponent@@UAE@XZ)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe @ 0x00495620, 344 bytes, ends in a tail jump to CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Header port (NF235, 2026-11-22, q12656): the local CBaseClassNonCopyable / CGameComponentInterface / CGameComponent / CWideString /
//   CWideStringVector / CFrontendGameComponent models replaced by the shared headers: engine/CGameComponent.h (DTOR_INLINE: the empty
//   inline ~CGameComponent, both base vtable stores then the tail jump to ~CBaseClassNonCopyable 0x0099A300), fable_string.h,
//   stlport/fable_containers.h (std::vector<CWideString>) and engine/CFrontendGameComponent.h (CTOR_DTOR: the typed layout; SETQUIT:
//   the slot-4 override). The two vtables now carry their PDB / RTTI names ??_7CFrontendGameComponent@@6BCBaseClassNonCopyable@@@
//   (0x01238768) / ??_7CFrontendGameComponent@@6BCDeviceResetCallback@@@ (0x0123875C) and their retail slots (all three PDBs; RTTI COL
//   0x013330F0 / 0x0133309C).
// Body: vtables 0x01238768 / 0x0123875C (this class at +0 / +4); ~CTexture 0x009F9F70 (SplashScreen); ~CWideString
// 0x0099B510 (WorldToLoad, SaveToLoad and every vector element); free 0x00BFEA14 (each vector's storage); the counted
// pointers' inline release (destroy function, operator delete 0x00BFE9BC, then Data and Info zeroed); base vtables
// 0x0122ECB0 / 0x0122ECA4.
// Names: the manifest's NHeroInformationScreens CBase destructor label is stale (Ego_r.pdb ??1CFrontendGameComponent).
// Layout: retail CFrontendGameComponent: PTempGraphics +0x1DFC, MainFont +0x1E04, ProfileNames +0x1E0C,
// ProfileNamesOffset +0x1E18, SaveNames +0x1E1C, SaveNamesOffset +0x1E28, SaveToLoad +0x1E2C, WorldNames +0x1E30,
// WorldNamesOffset +0x1E3C, WorldToLoad +0x1E40, CursorHighlight +0x1E44, TimeLastMovementHappened +0x1E48,
// StartedRepeatingMovement +0x1E50, CurrentMovement +0x1E54, SplashScreen +0x1E58 (Ego_r.pdb less 4 per STLport vector);
// the game-component base model of the landed ~CNewFrontendGameComponent 0x0042F50B.
// Shape: the vector<CWideString> destructor is inline: STLport _Destroy(first, last) over parameter copies (finish
// cached in ebx), then free of the reloaded start; the counted
// pointer is the CText model (Release, then Data = Info = 0).
// Opt: /O2 (16-aligned entry).
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
#include "engine/CGameComponent.h"
#include "engine/CFrontendGameComponent.h"

CFrontendGameComponent::~CFrontendGameComponent()
{
}
