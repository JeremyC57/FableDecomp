// CNewFrontendGameComponent::~CNewFrontendGameComponent() -- shut the front end down if that has not happened yet, then destroy the members (save name, file lists, banks, engine, def, init data, menu font) and the game-component base
// Leaf: ??1CNewFrontendGameComponent@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F50B, 158 bytes. Size TU (`#pragma optimize("s", on)`: `and dword ptr [esi+3Ch],0`).
// Header port (NF233, 2026-11-22, q12638; the CNewFrontendGameComponent vtable / ctor / dtor family): engine/CNewFrontendGameComponent.h
//   (RUN + FILENAMES_FILETIMES + the NF233 CTOR_DTOR opt-in: typed members, the ctor / dtor / Init / device-reset declarations, + SETQUIT
//   so slot 4 binds 0x00430340), engine/CGameComponent.h (NF233 DTOR_INLINE: ~CGameComponent inline, as Ego_r.pdb has no copy of it),
//   engine/CStopWatch.h (NF233 CTOR). Every local model of HEAD is gone (CBaseClassNonCopyable, the CGameComponentInterface stand-in base,
//   CGameComponent, CFrontEndDef, CDefPointer_CFrontEndDef, CMainGameComponentInit, CDateAndTime, CDateAndTimeVector, the class itself).
// Vtables (both emitted from the header build, the names retail's Ego_r.pdb / FableWin.pdb / Ego_d.pdb carry):
//   ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ 0x01230CA0 (was the same; slots ??_E 0x004330D2, Init 0x0042F75E, Run 0x0042EC7C,
//   ChangeTextureColourDepth 0x0042DC5C, SetQuit 0x00430340) and ??_7CNewFrontendGameComponent@@6BCDeviceResetCallback@@@ 0x01230C94 (was the
//   stand-in 6BCGameComponentInterface@@@; slots OnPreDeviceReset 0x0042DC5F, OnPostDeviceReset 0x0042DC60); the base's
//   ??_7CGameComponent@@6BCBaseClassNonCopyable@@@ 0x0122ECB0 / ??_7CGameComponent@@6BCDeviceResetCallback@@@ 0x0122ECA4 (was 6BCGameComponentInterface@@@).
// Callees: CNewFrontendGameComponent::Shutdown @ 0x0042EBB6, ~CWideString @ 0x0099B510, vector<CWideString>::~vector @ 0x00412130 (called: its
//   _Destroy loop is too big for the size TU), free @ 0x00BFEA14 (vector<CDateAndTime>'s destructor, expanded), the CCountedPointer<T>::ReleaseData
//   rows @ 0x00403750 / 0x0042F974 / 0x0042F9A0 / 0x0042918C / 0x00419108 / 0x0042F948 (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY, NF221),
//   ~CMainGameComponentInit @ 0x004131D0, the def's CDefPointer<const CFrontEndDef> release (header, inline), ~CBaseClassNonCopyable @ 0x0099A300
//   (tail jump after the inline ~CGameComponent's two vtable stores).
// Inline decisions (measured, NF233): at stock flags (no /d2inl flag, no __forceinline) the header ~CGameComponent {} and ~vector<CDateAndTime>
//   expand and ~vector<CWideString> is called, as in retail. HEAD's local models force-inlined both; nothing is forced now.
// Pragma scaffolding: `#pragma optimize("", on)` after the body restores the TU default before the pick-any copies (accepted form, nf226): the
//   ~vector<CWideString> COMDAT then equals its owner 0x00412130 (49 B; under the size mode it is a 30-B copy). ??_GCNewFrontendGameComponent
//   (owner 0x004330D2, 28 B, size) is generated with the vtable at the destructor and keeps the size mode.
// Quit (+8, CGameComponent, Ego_r.pdb) is the flag HEAD called m_bShutDown.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE
#define FABLE_RETAIL_CSTOPWATCH_CTOR
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CGameComponent.h"
#include "engine/CStopWatch.h"
#include "engine/CNewFrontendGameComponent.h"

CNewFrontendGameComponent::~CNewFrontendGameComponent()
{
    if (!Quit)
        Shutdown();
}
#pragma optimize("", on)
