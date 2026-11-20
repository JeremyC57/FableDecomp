// CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator()(CThing& test_thing) const -- thing filter: true when the thing has interface 0xC2 and that component's class type name is "CTCActionUseScriptedHook"
// Leaf: ??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z
// Flags: none
// Retail 0x00518DC0, 181 bytes (then int3 padding), `ret 4` (thiscall; the filter `this` is unused).
// Identity: Ego_r.pdb ??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z. Parameter and local: Ego_d.pdb (debug
//   length 40): this, test_thing, ptc_type. Ego_d C13 line table (486..493): 488 the GetTCOfLeafType test (19 bytes), 490 `return true;`
//   (4 bytes: mov al,1 + jmp), 492 `return false;` (2 bytes) -- the if / return form below.
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_STATCLASSTYPENAME
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/CThingFilter_HasTCOfLeafType.h"

template <class T> bool CThingFilter_HasTCOfLeafType<T>::operator()(CThing& test_thing) const
{
    T* ptc_type;
    if (test_thing.GetTCOfLeafType(&ptc_type))
        return true;
    return false;
}
template class CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>;