#ifndef FABLE_ENGINE_CINPUTPROCESSGAMEBASE_H
#define FABLE_ENGINE_CINPUTPROCESSGAMEBASE_H
#ifdef FABLE_RETAIL_INPUTPROCESS
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessGameBase : CAInputProcess, vtable 0x0125BE98;
// the base of every game input process. The constructor ??0CInputProcessGameBase@@QAE@ABVCInputProcessGameBaseInit@@@Z is out of
// line (retail 0x00687A30, landed; Ego_r.pdb and FableWin.pdb): it copies the four init pointers and clears PGamePlayerInterface.
// sizeof 0x28 (`push 28h ; call operator new` for the processes that add no member).
#include "rebuild_abi.h"
#include "engine/CAInputProcess.h"
#include "engine/CInputProcessGameBaseInit.h"
class CGameDefinitionManager;
class CGamePlayerInterface;
class CMainGameComponent;
class CPlayer;
class CWorld;
#ifdef FABLE_RETAIL_INPUTPROCESS_MOVEMENT
class C2DVector;
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#ifdef _MSC_VER
enum EInputButtonEventType;                                              // as engine/CGamePlayerInterface.h declares it
#else
enum EInputButtonEventType { };
#endif
#endif
#if defined(FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTASSIGNABLESPECIALMOVEBUTTON) || defined(FABLE_RETAIL_CINPUTPROCESSGAMEBASE_GETMOUSEWHEELMOVEMENT)
class CInputEvent;
#endif
#ifdef FABLE_RETAIL_COMBAT_INPUT_PASS26
#include "engine/EInputButtonEventType.h"
#include "engine/EGameAction.h"
class CInputEvent;
#endif
class CInputProcessGameBase : public CAInputProcess {
#ifdef FABLE_RETAIL_COMBAT_INPUT_PASS26
public:
    float GetButtonPressureForEvent(const CInputEvent& event) const;
protected:
    bool IsGameActionEventInQueue(EGameAction action,EInputButtonEventType type) const;
#endif
public:
    CInputProcessGameBase(const CInputProcessGameBaseInit& init);        // 0x00687A30
#ifdef FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
    // Nested opt-in (wf1, row 0x00688D40): Ego_r.pdb ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z, the override
    // out of line at 0x00687A70 (called by name from the CInputProcessZTarget override). Declaration only.
    virtual bool AddToInterface(CInputProcessManager& manager);          // 0x00687A70
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_ZTARGET
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;
protected:
    const CGamePlayerInterface& GetGamePlayerInterface() const { return *PGamePlayerInterface; }
    bool IsEventButtonPressed(const CInputEvent& event) const;
    bool IsEventButtonReleased(const CInputEvent& event) const;
public:
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_GETMOUSEMOVEMENT
    // Opt-in (NF62, row 0x00691AE0): Ego_r.pdb ?GetMouseMovement@CInputProcessGameBase@@QBE?AVC2DVector@@ABVCInputEvent@@@Z (Ego_r
    // 0x006B2E30; inline in retail, 0x00691BB8 copies the event's movement X / Y words: `mov eax,[esi+0Ch] ; mov ecx,[esi+10h]`). The
    // movement is Ego_r.pdb UInputEventData u_12.s_0.MouseMovement (C3DVector +0x0C; MousePos C2DVector +0x18), which engine/CInputEvent.h
    // keeps as raw bytes (a C++03 union cannot hold the PDB's vector members, they have constructors), so X / Y are read from Data.Raw.
    // Needs engine/C2DVector.h and engine/CInputEvent.h.
    C2DVector GetMouseMovement(const CInputEvent& event) const { return C2DVector(*(const float*)&event.Data.Raw[0x0C], *(const float*)&event.Data.Raw[0x10]); }
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_MOVEMENT
    // Retail opt-in (agent1022, row 0x00691D70 CInputProcessCreatureMovement::ProcessInput), declarations only:
    // Ego_r.pdb ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z
    // (retail 0x00687D00, landed under the made-up spelling GetEventGameActionVector) and
    // ?IsEventButtonHeld@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z (retail 0x00687B30).
    bool GetControlMovementFromGameActionEvent(EGameAction action, const CInputEvent& event, C2DVector& movement) const;   // 0x00687D00
protected:
    bool IsEventButtonHeld(const CInputEvent& event) const;                                                       // 0x00687B30
public:
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
    // Retail opt-in (w22 agent2007, row 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput), declaration only: Ego_r.pdb / FableWin.pdb
    // ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z, retail 0x00687D80 (`ret 0Ch`:
    // MyPlayer->GetNumber() 0x00487510, then PGamePlayerInterface->IsGameActionEventInQueue(action, type, number, time) 0x00445CB0).
    // The landed spelling ?IsGameActionEventInQueue@CInputProcessGameBase@@QAE_NJJPAM@Z (0x0068E450) is made up (NAME_FIXES.tsv).
protected:
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, float& time) const;                // 0x00687D80
public:
#endif
#ifdef FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISQUICKACCESSEVENTINQUEUE
    // NF55 opt-in (2026-11-17, row 0x0068E450 CInputProcessQuickAccessItems::Update), the retail-branch twin of the flat branch's
    // FABLE_RETAIL_INPUTPROCESS_QUICKACCESSQUEUE: Ego_r.pdb / FableWin.pdb ?IsQuickAccessEventInQueue@CInputProcessGameBase@@IBE_NJW4EInputButtonEventType@@@Z
    // (protected const, 0x00687CB0). Needs FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE (EInputButtonEventType). Declaration only.
protected:
    bool IsQuickAccessEventInQueue(long button, EInputButtonEventType type) const;   // 0x00687CB0
public:
#endif
#ifdef FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTASSIGNABLESPECIALMOVEBUTTON
    // Opt-in (NF478, 2026-11-26, row 0x00690030 CInputProcessSpecialAbilities::ProcessInput on this retail view), declaration only:
    // Ego_r.pdb (0x006B2D80) / FableWin.pdb (0x021D3FF0) ?IsEventAssignableSpecialMoveButton@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z
    // (protected const), retail 0x00687C70.
protected:
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event) const;                                         // 0x00687C70
public:
#endif
#ifdef FABLE_RETAIL_CINPUTPROCESSGAMEBASE_GETMOUSEWHEELMOVEMENT
    // Opt-in (NF478, 2026-11-26, row 0x00690030), declaration only: Ego_r.pdb (0x006B2E50) / FableWin.pdb (0x021D41C0)
    // ?GetMouseWheelMovement@CInputProcessGameBase@@QBEMABVCInputEvent@@@Z (public const), retail 0x00687D40 (landed).
    float GetMouseWheelMovement(const CInputEvent& event) const;                                                     // 0x00687D40
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_QUICKACCESS_PASS26
protected:
    bool IsEventQuickAccessItemButton(const CInputEvent& event,long& item_index) const;
    bool IsOtherQuickAccessItemButtonHeld(long item_index) const;
    bool IsEventContextSensitiveItemButton(const CInputEvent& event,long& item_index) const;
#endif
    CWorld*                 World;                                       // +0x14
    CPlayer*                MyPlayer;                                    // +0x18
    CGameDefinitionManager* DefinitionManager;                           // +0x1c
    CMainGameComponent*     Component;                                   // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;                        // +0x24
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x0125BE98, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
protected:
    virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent& event) const;             // slot 8, 0x00687DB0
public:
#endif
};
FABLE_STATIC_ASSERT_AT(67, sizeof(CInputProcessGameBase) == 0x28);
#else // the generated Ego_r layout (the landed default, unchanged)
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
#error FABLE_RETAIL_INPUTPROCESS_VTABLE needs FABLE_RETAIL_INPUTPROCESS (the retail class view)
#endif
#if defined(FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTASSIGNABLESPECIALMOVEBUTTON) || defined(FABLE_RETAIL_CINPUTPROCESSGAMEBASE_GETMOUSEWHEELMOVEMENT)
#error FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTASSIGNABLESPECIALMOVEBUTTON / _GETMOUSEWHEELMOVEMENT need FABLE_RETAIL_INPUTPROCESS (the retail class view)
#endif
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CInputProcessGameBase = 0x28.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CGameDefinitionManager;
class CGamePlayerInterface;
class CMainGameComponent;
class CPlayer;
class CWorld;
#ifdef FABLE_RETAIL_INPUTPROCESS_QUICKACCESSQUEUE
enum EInputButtonEventType;                                  // MS forward enum (FAM13 opt-in below)
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS
enum EGameAction;                                            // MS forward enums (NF37 opt-in below)
enum EInputButtonEventType;
class CInputEvent;
class C2DVector;
#endif

#ifdef FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON
class CInputEvent;                                           // NF64 opt-in below
#endif
#pragma pack(push, 1)
struct CInputProcessGameBase {
#ifdef FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON
    // NF64 opt-in (round 7, q10652: the CInputProcess* ProcessInput rows on this generated layout), the flat-branch twin of the retail
    // branch's FABLE_RETAIL_INPUTPROCESS_ZTARGET / _MOVEMENT button queries, as Ego_r.pdb (0x006B2BA0 / 0x006B2C40 / 0x006B2CE0) and
    // FableWin.pdb (0x021D3D20 / 0x021D3E10 / 0x021D3F00) spell them: ?IsEventButtonPressed / ?IsEventButtonHeld /
    // ?IsEventButtonReleased@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z (protected const). Declarations only, the layout is unchanged.
    // Held = 0x00687B30 (tests every IE_*_HELD code: 2/5/8/11/20/27..31), Released = 0x00687BD0 (IE_*_RELEASED: 3/6/9/12/21/32..36),
    // Ego_r.pdb EInputEvent; the same order as both PDBs. Landed swapped until 2026-11-22 (Codex pass 27 + chunk 829, land0711e).
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;                                                        // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event) const;                                                           // 0x00687BD0
    bool IsEventButtonHeld(const CInputEvent& event) const;                                                       // 0x00687B30
public:
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS
    // NF37 opt-in (the CInputProcess* ProcessInput / Update rows on this generated layout): the three queries the retail opt-ins
    // FABLE_RETAIL_INPUTPROCESS_ZTARGET / _MOVEMENT / _EVENTINQUEUE declare on the retail class, as Ego_r.pdb / FableWin.pdb spell them:
    // ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z (0x00687CF0),
    // ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z (0x00687D00),
    // ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z (0x00687D80, protected).
    // Declarations only, the layout below is unchanged.
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;                                       // 0x00687CF0
    bool GetControlMovementFromGameActionEvent(EGameAction action, const CInputEvent& event, C2DVector& movement) const;   // 0x00687D00
protected:
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, float& time) const;                // 0x00687D80
public:
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_QUICKACCESSQUEUE
    // Opt-in (FAM13, row 0x00687CB0): Ego_r.pdb / FableWin.pdb ?IsQuickAccessEventInQueue@CInputProcessGameBase@@IBE_NJW4EInputButtonEventType@@@Z
    // (protected const; forwards to CGamePlayerInterface::IsQuickAccessEventInQueue 0x004460C0 with MyPlayer's joystick device).
    // Declaration only, the layout below is unchanged.
protected:
    bool IsQuickAccessEventInQueue(long button, EInputButtonEventType type) const;   // 0x00687CB0
public:
#endif
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char           _pad_0x04[0x4];                  // +0x04
    unsigned char           _pad_0x08[0x4];                  // +0x08
    unsigned char           _pad_0x0c[0x4];                  // +0x0c
    unsigned char           _pad_0x10[0x4];                  // +0x10
    CWorld*                 World;                           // +0x14
    CPlayer*                MyPlayer;                        // +0x18
    CGameDefinitionManager* DefinitionManager;               // +0x1c
    CMainGameComponent*     Component;                       // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;            // +0x24
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(109, sizeof(CInputProcessGameBase) == 0x28);
FABLE_STATIC_ASSERT_AT(110, offsetof(CInputProcessGameBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(111, offsetof(CInputProcessGameBase, World) == 0x14);
FABLE_STATIC_ASSERT_AT(112, offsetof(CInputProcessGameBase, MyPlayer) == 0x18);
FABLE_STATIC_ASSERT_AT(113, offsetof(CInputProcessGameBase, DefinitionManager) == 0x1c);
FABLE_STATIC_ASSERT_AT(114, offsetof(CInputProcessGameBase, Component) == 0x20);
FABLE_STATIC_ASSERT_AT(115, offsetof(CInputProcessGameBase, PGamePlayerInterface) == 0x24);

#endif // FABLE_RETAIL_INPUTPROCESS
#endif // FABLE_ENGINE_CINPUTPROCESSGAMEBASE_H
