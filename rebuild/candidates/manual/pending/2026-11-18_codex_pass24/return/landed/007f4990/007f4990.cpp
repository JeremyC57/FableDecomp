// CTCHasName::GetNPCShortName() -- use the stored name or resolve and store a name from the text bank.
// Leaf: ?GetNPCShortName@CTCHasName@@QAE?AVCWideString@@XZ
// Retail: 0x007F4990, 449 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_GETTHINGMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
#define FABLE_RETAIL_CTHINGMANAGER_NPCSHORTNAME
#define FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#define FABLE_RETAIL_CHASNAMEDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CHasNameDef.h"
#include "engine/CTCHasName.h"
#include "engine/CThingManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

CWideString CTCHasName::GetNPCShortName()
{
    if (GetThingManager().DoesNPCNameEntryExistForThing(ParentThing->GetUniqueID())) {
        CWideString name = GetThingManager().GetNameForThing_OnlyCalledFromTCHasName(ParentThing->GetUniqueID());
        return name;
    } else {
        const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
        CWideString default_name_converted;
        if (text_bank.EntryExists(PDef->DefaultNameTag)) {
            if (text_bank.GetEntryDataType(PDef->DefaultNameTag) == 0) {
                CIVCountedPointer<const NGameText::CTextEntry> ptext_entry = text_bank.GetPTextEntry(PDef->DefaultNameTag, true);
                default_name_converted = ptext_entry->GetText();
            } else {
                CIVCountedPointer<const NGameText::CGroupEntry> pgroup_entry = text_bank.GetPGroupEntry(PDef->DefaultNameTag, true);
                default_name_converted = text_bank.GetPTextEntry(pgroup_entry->Entries[rand() % pgroup_entry->Entries.size()], true)->GetText();
            }
            GetThingManager().SetNameForThing(ParentThing->GetUniqueID(), default_name_converted);
        }
        return default_name_converted;
    }
}
