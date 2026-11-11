// CTCInventoryQuests::OnCreate() -- add the quests screen to the hero-information screen manager, point the inventory at the creature def's quests-screen inventory def, run the base OnCreate, size the quest-card primitive handles (4) and the quest-card item list (6), reset the item selection, and fill the six quest-card items with their index and header text
// Leaf: ?OnCreate@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061A6A0, 484 bytes of code to `ret` at 0x0061A883 + the 6-entry jump table 0x0061A884..0x0061A89B
//   (the catalog's 512 includes 4 int3); thiscall, `and esp,-8` frame (the 8-byte CQuestCardItem local), no EH frame.
// Identity: vtable 0x012585B4 (RTTI CTCInventoryQuests) slot 4 = 0x0061A6A0, CTCBase's OnCreate slot; Ego_r.pdb / FableWin.pdb
//   ?OnCreate@CTCInventoryQuests@@UAEXXZ; it calls NInventory::CTCInventoryBase::OnCreate 0x005C0121 qualified, as the landed sibling
//   screens (CTCInventoryWeapons 0x005C3057, CTCInventoryMap 0x00602E30) do.
// Callees: CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 on CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790,
//   NHeroInformationScreens::CManager::RegisterScreen 0x0066EB11 (the null-safe CBase conversion, +0x130),
//   CDefClassBase::GetSubDef<CCreatureDef> 0x00485815 on the thing's PBaseDef (a CDefPointer copy, add-ref'd / released around it),
//   NInventory::CTCInventoryBase::SetPDef 0x005BDB4D with CCreatureDef::QuestsScreenInventory (+0xA4) and OnCreate 0x005C0121,
//   vector<CEnginePrimitiveHandle>::resize 0x00443F90, vector<CQuestCardItem>::resize 0x006253D0, CWideString() 0x0099AED0 /
//   CWideString(const unsigned short*) 0x0099B6B0 / operator= 0x0099B7D0 / ~CWideString 0x0099B510; the defs' slot 1 (Delete).
// Globals: the quest-card item selection index 0x013B8C64 and count 0x013B8C68 (module statics; the established descriptive
//   spellings g_testQuestIndex / g_testQuestCount; IncrementQuestCardItemSelection 0x0061A1A0 wraps the index at the count).
// Shape: the base def is a GetPBaseDef() copy (add-ref / release around GetSubDef); the same-TU GetSubDef<CCreatureDef> body lets
//   VC7.1 keep the sub-def in ebp and drop the final release's null test; the item loop is a do-while (no aligned loop head, i = 0 in
//   esi before the statics are stored); each header text is a block-scoped CWideString copy-initialised from the literal (the
//   implicit conversion re-takes the temporary's address with `lea` after 0x0099B6B0; `CWideString(L"...")` pushes eax instead).
// Headers: FABLE_HDR_OVERLAY=$A/hdr (CTCInventoryQuests.h, CQuestCardItem.h, CCreatureDef.h, NInventory_CTCInventoryBase.h,
//   NHeroInformationScreens_CManager.h opt-ins).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ONCREATE
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CPLAYERGUI
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CPlayerGui.h"

extern long g_testQuestIndex;                    // 0x013B8C64 (quest-card item selection index; established spelling)
extern long g_testQuestCount;                    // 0x013B8C68 (quest-card item count; established spelling)

// GetSubDef<CCreatureDef> 0x00485815 (the landed CTCTalk::OnCreate 0x006E2C00 body): retail instantiated it in this TU, so VC7.1
// sees that the out slot is not captured and keeps the sub-def pointer in ebp to the end, releasing it without a null test
// (0x0061A701 `mov ebp,[esp+10h]` ... 0x0061A86F `dec dword ptr [ebp+4]`). Kept out of line, as retail calls it.
template <>
bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCreatureDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

enum { QUEST_CARD_SELECT_PRIMITIVE_COUNT = 4, QUEST_CARD_ITEM_COUNT = 6 };

void CTCInventoryQuests::OnCreate()
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefPointer<const CCreatureDef> creature_def;
    GetParentThing().GetPBaseDef()->GetSubDef(&creature_def);
    SetPDef(creature_def->QuestsScreenInventory);
    NInventory::CTCInventoryBase::OnCreate();
    InventoryQuestCardSelectPrimitiveHandles.resize(QUEST_CARD_SELECT_PRIMITIVE_COUNT);
    QuestCardItemList.resize(QUEST_CARD_ITEM_COUNT);
    g_testQuestCount = QUEST_CARD_ITEM_COUNT;
    g_testQuestIndex = 0;
    CQuestCardItem item;
    long i = 0;
    do {
        item.ItemIndex = i;
        switch (i) {
        case 0: { CWideString text = L"Description"; item.ItemText = text; } break;
        case 1: { CWideString text = L"Picture"; item.ItemText = text; } break;
        case 2: { CWideString text = L"Trophy"; item.ItemText = text; } break;
        case 3: { CWideString text = L"Location"; item.ItemText = text; } break;
        case 4: { CWideString text = L"Rewards"; item.ItemText = text; } break;
        case 5: { CWideString text = L"Prerequisites"; item.ItemText = text; } break;
        }
        QuestCardItemList[i] = item;
    } while (++i < QUEST_CARD_ITEM_COUNT);
    QuestCardCount = 0;
}