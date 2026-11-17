// CThingPlayerCreature::PreOnKill() -- for the main player: use a resurrection item from the inventory when one is there, else open the quest-completion UI in "hero died" mode; either way the kill is vetoed
// Leaf: ?PreOnKill@CThingPlayerCreature@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB300, length 302 bytes (+ int3 padding).
// Callees: CThing::GetPlayer @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
//   (interfaces 0x11 = the inventory, 0xE1 = CTCResurrectionItem, 0xFC = CTCQuestCompletionUI), NInventory::CTCInventoryBase::
//   GetResurrectionItemInInventory @ 0x005BF77D, CTCResurrectionItem::OnItemUsed(CThingCreatureBase&) @ 0x007F04C0, CWideString(const wchar_t*)
//   @ 0x0099B6B0 / ~CWideString @ 0x0099B510, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CTCQuestCompletionUI::Activate(CompletionType, const CCharString&, bool, const CWideString&, bool) @ 0x005E4740. Strings: 0x0122D70C L"",
//   0x0122D70E "".
// Shape: the inventory lookup is the plain entry-pick GetTC used straight as `this`; the resurrection item and the UI are
//   `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the zero preload); both Activate strings are implicit conversion temporaries (wide first, by
//   right-to-left evaluation); the main-player test wraps the body so the `return true` exit is laid out last.
// Name fix (agentNF91, q11200 / q11205): Ego_r.pdb (0x00549470) + FableWin.pdb ?PreOnKill@CThingPlayerCreature@@MAE_NXZ (protected virtual, the
//   CThing slot-13 override: vtable 0x012457FC slot 13 = 0x006AB300); the old row defined a public non-virtual QAE spelling on local CPlayer /
//   CThingCreatureBase / CThingPlayerCreature / CTCInventoryBase / CTCQuestCompletionUI models and bound 0x005BF77D as
//   ?GetResurrectionItemInInventory@CTCInventoryBase@@QAEPAVCThing@@XZ. Now on the shared headers (CThingPlayerCreature.h + the NF91 overlay
//   opt-in FABLE_RETAIL_CTHINGPLAYERCREATURE_PREONKILL, CPlayer.h, NInventory_CTCInventoryBase.h + RESURRECTIONITEM, CTCQuestCompletionUI.h);
//   CTCResurrectionItem has no shared header (declared here with its one Ego_r.pdb / FableWin.pdb member); the empty literals are written as
//   literals (the old row named 0x0122D70C g_emptyWideString_0122d70c).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_PREONKILL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_RESURRECTIONITEM
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_GLOBALUPDATE
#include "fable_string.h"
#include "engine/CPlayer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCQuestCompletionUI.h"
class CTCResurrectionItem {                                     // no shared header (RTTI .?AVCTCResurrectionItem@@)
public:
    void OnItemUsed(CThingCreatureBase& creature);              // 0x007F04C0 (Ego_r.pdb + FableWin.pdb QAEXAAVCThingCreatureBase@@@Z)
};

bool CThingPlayerCreature::PreOnKill()
{
    if (GetPlayer().IsMainPlayer()) {
        if (HasTC(TCI_INVENTORY)) {
            CThing* item = static_cast<NInventory::CTCInventoryBase*>(&GetTC(TCI_INVENTORY))->GetResurrectionItemInInventory();
            if (item && !item->IsDead()) {
                CTCResurrectionItem* resurrection = 0;
                if (item->HasTC(TCI_RESURRECTION_ITEM))
                    resurrection = (CTCResurrectionItem*)&item->GetTC(TCI_RESURRECTION_ITEM);
                resurrection->OnItemUsed(*this);
                return false;
            }
        }
        CTCQuestCompletionUI* ui = 0;
        if (HasTC(TCI_QUEST_COMPLETION_UI))
            ui = (CTCQuestCompletionUI*)&GetTC(TCI_QUEST_COMPLETION_UI);
        ui->Activate(CTCQuestCompletionUI::HERO_DIED, "", true, L"", true);
        return false;
    }
    return true;
}
