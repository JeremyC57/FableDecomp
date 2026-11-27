// 0071d020 CTCHeroMorph::OnSerialise
// Flags:
// Inline-budget: the framed vector overflow at 005b3275 expands its allocation. See the original helper receipts.
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_ORIGINAL_PASS26
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
// Leaf: ?OnSerialise@CTCHeroMorph@@UAEXAAVCPersistContext@@_N@Z
#define FABLE_RETAIL_HERO_MORPH_SERIALISE_PASS29
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDataOutputStream.h"
#include "engine/CDefString.h"
#include "engine/fable_script_text.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CWorld.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"

#include "engine/CTCHeroMorph_SerialiseOwners.h"

void CTCHeroMorph::OnSerialise(CPersistContext& context,bool in_game)
{
    context.Transfer("Strength", Strength);
    context.Transfer("Will", Will);
    context.Transfer("Skill", Skill);
    context.Transfer("Age", Age);
    context.Transfer("Morality", Morality);
    context.Transfer("Fatness", Fatness);
    context.Transfer("Tan", Tan);
    context.Transfer("Teenager", Teenager);
    if(context.IsSaving()) {
        long number_of_tattoos=Tattoos.size();
        context.Transfer("NumberOfTattoos",number_of_tattoos);
        for(long c1=0;c1<number_of_tattoos;++c1) {
            CCharString def_name=Tattoos[c1]->GetInstantiationName();
            context.Transfer("Tattoo"+GFIntToCharString(c1),def_name);
        }
    } else {
        std::vector<CDefPointer<const CThingBaseDef> > temp_tattoos;
        std::vector<CDefPointer<const CThingBaseDef> >& tattoos_ref=PeekWorld().IsSerialisingAboutToLoadHeroState()?temp_tattoos:Tattoos;
        long number_of_tattoos;
        context.Transfer("NumberOfTattoos",number_of_tattoos);
        tattoos_ref.reserve(number_of_tattoos);
        for(long c1=0;c1<number_of_tattoos;++c1) {
            CCharString def_name;
            context.Transfer("Tattoo"+GFIntToCharString(c1),def_name);
            CDefPointer<const CThingBaseDef> pthing_def;
            if(GetDefinitionManager().GetDef(def_name,&pthing_def))tattoos_ref.push_back(pthing_def);
        }
    }
}