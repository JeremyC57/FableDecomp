// CAIStateGroup_CreatureUseCoverBase::RefreshCoverList() -- refill retained cover handles from a filtered temporary list.
// Leaf: ?RefreshCoverList@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009710D0, 250 bytes.
// Identity (NF56, round 7, queue q10459): Ego_r.pdb 0x00995E60 / FableWin.pdb 0x02B858C0 ?RefreshCoverList@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ
//   (protected, non-virtual). Was the stand-in UseCoverMethods::RefreshCoverList (a local struct derived from the flattened header
//   layout, dispatching through a local 24-slot CCoverDispatch view). Now on the retail class of engine/CAIStateGroup_CreatureUseCoverBase.h
//   (FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE, opt-in _COVERLIST): the two calls are the class's own virtual slots 23 (+0x5C)
//   GetCoverThingList and 22 (+0x58) ProcessCoverThingList (retail vtable 0x0128D31C: 0x00970DF0 / 0x00902F9A), both taking the
//   PDB vector<CThing*>&; the temporary is a stack std::vector<CThing*> (STLport 4.6, __malloc_alloc: `free`).
// CoverList (opaque 0xC bytes in the header) is the PDB vector<CIntelligentPointer<CThing> >: clear() calls the out-of-line STLport
//   erase(first, last) 0x004AC0F0 and push_back's overflow 0x004AC940 (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, retail
//   CIntelligentPointer copy FABLE_RETAIL_CINTELLIGENTPOINTER_COPY: ctor 0x00A01B10, SetPItem 0x00A01B90, dtor 0x00A01C10).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#include "fable_compiler.h"
#include <stddef.h>
#include <stdlib.h>
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"

void CAIStateGroup_CreatureUseCoverBase::RefreshCoverList()
{
    std::vector<CIntelligentPointer<CThing> >& covers = *(std::vector<CIntelligentPointer<CThing> >*)CoverList;
    covers.clear();
    ValidPositionsFound = 0;
    std::vector<CThing*> things;
    GetCoverThingList(things);
    if (!things.empty()) {
        ProcessCoverThingList(things);
        for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it)
            covers.push_back(CIntelligentPointer<CThing>(*it));
    }
}
