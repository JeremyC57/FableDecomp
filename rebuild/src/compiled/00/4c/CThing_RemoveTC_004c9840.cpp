// CThing::RemoveTC(ETCInterfaceType type) -- deferred removal of a thing component: find the TC of `type`; unless already flagged, flag it,
//   clear its presence bit, queue {this, tc} on CThing::TCsToBeRemoved and notify the TC (OnRemove)
// Leaf: ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9840, 224 bytes incl. padding (ret 4).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline Find), the TC's virtual slot 21 GetInterfaceType,
//   std::bitset<274>::reset(size_t) 0x004CC0D0 (out of line), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor
//   0x00A01C10 (the CIntelligentPointer<CThing>(this) temporary and the pair copy), list::insert(iterator, const value_type&) 0x004CCC80 on
//   CThing::TCsToBeRemoved 0x013B8A24 (push_back), the TC's virtual slot 2 OnRemove.
// Identity: Ego_r.pdb / FableWin.pdb ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z; Ego_d.pdb names the parameter `type` and the locals it,
//   ptc (inner scope).
// Port (NF92, q11218 binder): the local CThing / CTCBase / CVectorMap / bitset / list / CIntelligentPointer models (it bound the list as the
//   made-up ?g_TCRemovalList@@3UCTCRemovalList@@A) moved onto engine/CThing.h (NF90 opt-in FABLE_RETAIL_CTHING_TCSTOBEREMOVED), CTCBase.h,
//   CIntelligentPointer.h (COPY: the pair copy goes through GetPItem / SetPItem) and stlport/fable_containers.h (STLP_LIST_INSERT_OUTOFLINE,
//   NF92 opt-in STLP_BITSET_RESET_DECL).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_TCSTOBEREMOVED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_BITSET_RESET_DECL
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"

void CThing::RemoveTC(ETCInterfaceType type)
{
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = Components.Find(type);
    if (it != Components.end()) {
        CTCBase* ptc = it->second;
        if (!ptc->ToBeRemoved) {
            ptc->ToBeRemoved = true;
            UsingComponents.reset(ptc->GetInterfaceType());
            TCsToBeRemoved.push_back(std::pair<CIntelligentPointer<CThing>, CTCBase*>(CIntelligentPointer<CThing>(this), ptc));
            ptc->OnRemove();
        }
    }
}
