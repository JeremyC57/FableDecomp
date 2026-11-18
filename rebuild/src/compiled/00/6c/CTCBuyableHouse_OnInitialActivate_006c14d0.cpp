// CTCBuyableHouse::OnInitialActivate() -- re-take the buyable-house sub-def, make sure the parent has a thing-owner component, find the house's sign posts among its owned things (by def global index), remap the wife pointer after loading, kill a live main sign post and create a fresh one when the house needs it
// FITTED: the implicit const char* -> CCharString AddTC argument (retail builds the temporary before the 0 / false pushes and reloads ParentThing after it, 0x006C153A..0x006C1556; the explicit CCharString(...) spelling pushes first). Assignments use the shared FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN body (GetPItem on the temporary, SetPItem; 0x006C1646). The same-TU GetDef<CThingBaseDef> / GetSubDef<CBuyableHouseDef> bodies are the shared noinline ones (called out of line at 0x006C150A / 0x006C1519).
// Leaf: ?OnInitialActivate@CTCBuyableHouse@@UAEXXZ
// Inline-budget: existing shared noinline bodies FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY / FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY -- retail calls GetDef<CThingBaseDef> 0x0042AF3C and GetSubDef<CBuyableHouseDef> 0x006C1B00 out of line (0x006C150A / 0x006C1519); the visible bodies give retail's `dec [edi+4]` release of pdef.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C14D0, 734 bytes (8-aligned ebp frame, `ret`).
// Ego_d.pdb (tc_buyable_house.obj): ptc_thing_owner, main_signpost_def_index, main_owned_def_index, main_rent_def_index,
//   dressing_signpost_def_index at function scope; { pdef (CDefPointer<const CThingBaseDef>) }; { end_it, it { pthing } }.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHINGOWNER
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_ONINITIALACTIVATE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CBuyableHouseDef.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/ThingAlive.h"
namespace { extern const CCharString MAIN_SIGN_POST_DEF_NAME; }          // 0x013BAAF8 "OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST"
namespace { extern const CCharString MAIN_OWNED_SIGN_POST_DEF_NAME; }    // 0x013BAB00
namespace { extern const CCharString MAIN_RENT_SIGN_POST_DEF_NAME; }     // 0x013BAB04
namespace { extern const CCharString DRESSING_SIGN_POST_DEF_NAME; }      // 0x013BAAFC
void FABLE_FASTCALL GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& ptr);   // 0x0088AFF0 Ego_r / FableWin ?GFRemapToThingAfterSerialisation@@YIXAAV?$CIntelligentPointer@VCThing@@@@@Z (Ego_d parameter ptr)

void CTCBuyableHouse::OnInitialActivate()
{
    CTCThingOwner* ptc_thing_owner;
    if (GetParentThing().GetDefGlobalIndex())
    {
        CDefPointer<const CThingBaseDef> pdef;
        GetDefinitionManager().GetDef(GetParentThing().GetDefGlobalIndex(), &pdef);
        pdef->GetSubDef(&PDef);
    }
    if (!GetParentThing().HasTC(TCI_THING_OWNER))
        GetParentThing().AddTC("CTCThingOwner", false, 0);
    long main_signpost_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_SIGN_POST_DEF_NAME);
    long main_owned_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_OWNED_SIGN_POST_DEF_NAME);
    long main_rent_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_RENT_SIGN_POST_DEF_NAME);
    long dressing_signpost_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(DRESSING_SIGN_POST_DEF_NAME);
    ptc_thing_owner = 0;
    GetParentThing().GetTC(TCI_THING_OWNER, &ptc_thing_owner);
    {
        std::list<CIntelligentPointer<CThing> >::const_iterator end_it = ptc_thing_owner->GetObjectList().end();
        std::list<CIntelligentPointer<CThing> >::const_iterator it;
        for (it = ptc_thing_owner->GetObjectList().begin(); it != end_it; ++it)
        {
            CThing* pthing = it->Get();
            if (pthing)
            {
                if (pthing->GetDefGlobalIndex() == main_signpost_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == main_owned_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == main_rent_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == dressing_signpost_def_index)
                    DressingSignPost = CIntelligentPointer<CThing>(pthing);
            }
        }
    }
    GFRemapToThingAfterSerialisation(WifeLivingHere);
    if (IS_THING_ALIVE(MainSignPost))
    {
        MainSignPost->Kill(true);
        MainSignPost = CIntelligentPointer<CThing>(0);
    }
    if (NeedsMainSignPost() && !IS_THING_ALIVE(MainSignPost))
        CreateMainSignPost();
}