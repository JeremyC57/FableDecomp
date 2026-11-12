// CThingPlayerCreature::LearnExpression(const CCharString& expressionName, long quickAccessSlot, bool silent) -- unless the expression is already available: look its CExpressionDef up by name (global index > 0, class type "EXPRESSION"), spawn the def's DummyObject inventory object at the hero, put it in the hero's inventory, record the learned expression, give it a quick-access slot and (unless silent) the item's tutorial category; false when any step fails (a spawned object without the expression component is killed)
// Leaf: ?LearnExpression@CThingPlayerCreature@@QAE_NABVCCharString@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC430, length 606 bytes (`ret 0xc`). Ego_r.pdb / FableWin.pdb same mangled name.
// Callees: CThingPlayerCreature::IsExpressionAvailable @ 0x006AC390, CThing::PeekDefinitionManager @ 0x004C7990,
//   CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 / GetDefClassTypeNameFromGlobalIndex @ 0x009ACCE0 (CDefString by
//   value) / GetDef<CExpressionDef>(long, CDefPointer<const CExpressionDef>*) const (FableWin.pdb
//   ??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z) @ 0x005F81BE, one COMDAT-folded
//   body shared by 13 instantiations (manifest label GetDef<CSpecialAbilitiesDrunkennessDef>) -> GetPDefFromGlobalIndex
//   @ 0x009AD9E0, CDefString::operator==(const char*) @ 0x005B3440 on "EXPRESSION" @ 0x01235D94, CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("" @ 0x0122D70E), CThing::GetPos @ 0x004C73D0, CThingObject::Create @ 0x00703210
//   (static __fastcall), CThing::Kill @ 0x004C9B80, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (GetTC),
//   CTCInventoryItem::AddToInventory @ 0x005D8D50 / GetTutorialCategory @ 0x005D92B0, CTCInventoryAbilities::AddLearnedExpression
//   @ 0x005EEDCF, CTCInventory::GetQuickAccessNextFreeSlot @ 0x00582027 / SetThingAsQuickAccessItem @ 0x00584157,
//   CTCHero::GiveTutorialForCategory @ 0x006C9FA0; the def's release (virtual slot 1 at a zero count).
// Shape: the template GetDef<T> body is visible to the compiler (header opt-in GETDEF_GLOBAL_BODY: template, __declspec(noinline),
//   emitted out of line as a COMDAT, folded with 12 other instantiations at 0x005F81BE), so VC7.1
//   knows the def slot is not captured: the def is loaded once into ebx, released from ebx on the shared fail path and from its
//   home on the success path, with no null test (dereferenced first). DummyObject is a CDefIndex (Ego_r.pdb member type): its inline
//   operator long is evaluated before GetPos(), which puts item / map / inventory in ebp / edi / ebx as retail. Component lookups
//   are the forced-inline CThing::GetTC<T>(T**) with the presence bit test and the out-of-line LowerBound.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_EXPRESSIONS
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CEXPRESSIONDEF_DEFINDEX
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC
#define FABLE_RETAIL_CTCINVENTORY_NEXTFREESLOT
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCINVENTORYITEM_TUTORIALCATEGORY
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEARNEDEXPRESSION
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCHERO_TITLE
#include "stlport/fable_containers.h"
#include "engine/fable_nscript_forward.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CExpressionDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCHero.h"

bool CThingPlayerCreature::LearnExpression(const CCharString& expressionName, long quickAccessSlot, bool silent)
{
    if (!IsExpressionAvailable(expressionName)) {
        long expression = PeekDefinitionManager().GetDefGlobalIndexFromName(expressionName);
        if (expression > 0) {
            if (PeekDefinitionManager().GetDefClassTypeNameFromGlobalIndex(expression) == "EXPRESSION") {
                CDefPointer<const CExpressionDef> def;
                PeekDefinitionManager().GetDef(expression, &def);
                CThingObject* item;
                {
                    CCharString name("");
                    item = CThingObject::Create(def->DummyObject, GetPos(), PlayerNumber, 0, 0, name);
                }
                if (item) {
                    if (item->HasTC(TCI_EXPRESSION)) {
                        CTCInventory* inventory;
                        if (GetTC(&inventory)) {
                            CTCInventoryItem* itemTC = 0;
                            item->GetTC(&itemTC);
                            itemTC->AddToInventory(*inventory, false, true, 0, silent, false);
                            CTCInventoryAbilities* abilities;
                            if (GetTC(&abilities))
                                abilities->AddLearnedExpression(expression);
                            if (quickAccessSlot != -1 || (quickAccessSlot = inventory->GetQuickAccessNextFreeSlot(item->GetDefGlobalIndex())) != -1)
                                inventory->SetThingAsQuickAccessItem(item->GetDefGlobalIndex(), quickAccessSlot, false);
                            CTCHero* hero;
                            if (!silent && GetTC(&hero) && hero && itemTC->GetTutorialCategory())
                                hero->GiveTutorialForCategory(itemTC->GetTutorialCategory(), false);
                            return true;
                        }
                    } else
                        item->Kill(false);
                }
            }
        }
    }
    return false;
}