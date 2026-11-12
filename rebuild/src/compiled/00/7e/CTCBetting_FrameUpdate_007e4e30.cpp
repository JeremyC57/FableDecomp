// CTCBetting::FrameUpdate() -- follow the global betting flag. On a change to active: enter player mode 39 (betting) if not in it,
// zero the bet, and (when the parent thing's def has a CBettingDef sub-def) show the betting question with the continue/cancel
// buttons in the subtitle renderer and post game event 9 carrying `true`; then schedule FrameToStart = server frame + start delay
// seconds * ConstantFPS. On a change to inactive: leave mode 39 (posting event 9 with `false`) and Clear() every bet and money
// primitive handle.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4E30, 645 bytes, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
//   Vtable CTCBetting 0x01270624 slot 9; Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCBetting@@UAEXXZ.
// Callees: CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetCurrentMode 0x00633BE0 /
//   AddMode 0x006345C0 / RemoveMode 0x00634190, CDefClassBase::GetSubDef<CBettingDef> 0x007E5820 (landed body, defined here as in
//   retail's TU), CWideString(const wchar_t*) 0x0099B6B0 / ~CWideString 0x0099B510, CTCBetting::GetText 0x007E4D70,
//   CSubtitleRenderer::SetText (text + 3 buttons) 0x00492F69, CGameEvent::AddData 0x009F1760, CMainGameComponent::AddGameEvent
//   0x0041649C / GetCurrentServerFrame 0x00416225; slots 1/3 of the def and of CEnginePersistentPrimitiveBase (Release / NotifyReleased).
// Globals: PMainGameComponent 0x013B86A0, ConstantFPS 0x01375550, CPlayerGui PClass 0x013B8790, g_bettingActive 0x013BAE1D,
//   g_bettingAmount 0x013BAE20, g_bettingStartDelaySeconds 0x01384050 (descriptive: no name in any PDB / FSE / string).
// Shape: the L"" button caption is a named local in its own block, so it is built first (into the dead def slot) and destroyed
//   right after SetText, before the event; the handle loops are the inline CEnginePrimitiveHandle::Clear() (notify at count 1, Reset(0)).
// Leaf: ?FrameUpdate@CTCBetting@@UAEXXZ
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CBETTINGDEF
#define FABLE_RETAIL_CTCBETTING
#define FABLE_RETAIL_CTCBETTING_FRAMEUPDATE
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CBettingDef.h"
#include "engine/CDefinitionManager.h"
template <>
bool CDefClassBase::GetSubDef<CBettingDef>(CDefPointer<const CBettingDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CBettingDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CTCBetting.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameEvent.h"

extern bool g_bettingActive;                            // 0x013BAE1D
extern long g_bettingAmount;                            // 0x013BAE20
extern long g_bettingStartDelaySeconds;                 // 0x01384050 (descriptive)

void CTCBetting::FrameUpdate()
{
    if (Active == g_bettingActive)
        return;
    Active = g_bettingActive;
    CPlayer& player = GetPlayerManager().GetMainPlayer();
    if (Active) {
        if (player.GetCurrentMode() != PLAYER_MODE_BETTING)
            player.AddMode(PLAYER_MODE_BETTING, false);
        g_bettingAmount = 0;
        CDefPointer<const CBettingDef> def;
        if (ParentThing->PeekBaseDef().GetSubDef(&def)) {
            {
                CWideString empty(L"");
                CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekMainFont(), GetText(def->Question),
                    empty, GetText(def->Continue), GetText(def->Cancel), 0x404, 0.0f, false, true);
            }
            CGameEvent event(9);
            event.AddUBYTE(true);
            CMainGameComponent::Get()->AddGameEvent(event);
        }
        FrameToStart = CMainGameComponent::Get()->GetCurrentServerFrame() + g_bettingStartDelaySeconds * CMainGameComponent::GetConstantFPS();
    } else {
        if (player.GetCurrentMode() == PLAYER_MODE_BETTING) {
            player.RemoveMode(PLAYER_MODE_BETTING);
            CGameEvent event(9);
            event.AddUBYTE(false);
            CMainGameComponent::Get()->AddGameEvent(event);
        }
        for (std::vector<CEnginePrimitiveHandle>::iterator it = BetHandles.begin(); it != BetHandles.end(); ++it)
            it->Clear();
        for (std::vector<CEnginePrimitiveHandle>::iterator it2 = MoneyHandles.begin(); it2 != MoneyHandles.end(); ++it2)
            it2->Clear();
    }
}