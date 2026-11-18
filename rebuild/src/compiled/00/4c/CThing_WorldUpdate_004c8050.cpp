// CThing::WorldUpdate() -- frame-update every component on the update list that is not to be removed while the thing is alive; then sweep
//   the pending-removal list CThing::TCsToBeRemoved: drop entries whose thing is gone or dead, and for this thing's entries unhook the
//   component (driver pointer, update list, interface map), delete it and drop the entry; always false
// Leaf: ?WorldUpdate@CThing@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8050, 393 bytes, plain `ret`; CThing vtable slot 19.
// Name: Ego_r.pdb `?WorldUpdate@CThing@@UAE_NXZ` + FableWin.pdb (slot 19, engine/CThing.h FABLE_RETAIL_CTHING_ORIGINAL_VTABLE). Locals and scopes
//   from Ego_d.pdb (egod_locals.py): `it` / `pbase` in the update loop block, `it` / `ptc_base` / `components_it` / `tc_interface_type` in the
//   sweep block.
// Callees: CTCBase slots 9 FrameUpdate (+0x24), 24 IsDriver (+0x60), 21 GetInterfaceType (+0x54) and the scalar deleting destructor (+0);
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3: IS_THING_ALIVE's null test and IsDead, then the owner compare) and
//   ~CBaseIntelligentPointer 0x00A01C10 (list erase); list<CTCBase*>::remove 0x004CC150; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   0x0040F020 (inside the inline Find); free 0x00BFEA14 (the pending-removal node).
// Data: CThing::TCsToBeRemoved 0x013B8A24 (q11218 spelling, engine/CThing.h FABLE_RETAIL_CTHING_TCSTOBEREMOVED).
// Shape: the sweep tests the entry through the original IS_THING_ALIVE<CIntelligentPointer<CThing> > (engine/ThingAlive.h, FableWin.pdb
//   0x01CCA150), which puts the merged erase at the failed test as retail does; list<CTCBase*>::remove is the implicit same-TU instance
//   of the STLport _list.c body (NF92 opt-in FABLE_RETAIL_STLP_LIST_REMOVE_NOINLINE), so VC7.1 keeps ptc_base in ebp across the call and
//   shares the argument's frame word with the Find key, and `this` is the spilled frame word (0x004C805E).
// Port (NF92, q11218 binder): was a local July model (CRetailList / CInterfaceMap / STCRemoval, same-TU noinline stand-ins for remove and
//   LowerBound, a descriptive IsThingAlive) binding 0x013B8A24 as ?TCsToBeRemoved@CThing@@2V?$CRetailList@USTCRemoval@@@@A; now on the shared
//   headers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_TCSTOBEREMOVED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_NOINLINE
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/ThingAlive.h"

bool CThing::WorldUpdate()
{
    {
        for (std::list<CTCBase*>::iterator it = UpdateComponentsList.begin(); it != UpdateComponentsList.end(); ++it) {
            if (IsDead())
                return false;
            CTCBase* pbase = *it;
            if (!pbase->IsToBeRemoved())
                pbase->FrameUpdate();
        }
    }
    if (IsDead())
        return false;
    {
        std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> >::iterator it = TCsToBeRemoved.begin();
        while (it != TCsToBeRemoved.end()) {
            if (!IS_THING_ALIVE(it->first))
                it = TCsToBeRemoved.erase(it);
            else if (it->first.Get() == this) {
                CTCBase* ptc_base = it->second;
                if (ptc_base) {
                    if (ptc_base->IsDriver())
                        PTCDriver = 0;
                    UpdateComponentsList.remove(ptc_base);
                    ETCInterfaceType tc_interface_type = ptc_base->GetInterfaceType();
                    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator components_it = Components.Find(tc_interface_type);
                    while (components_it != Components.end() && components_it->second != ptc_base && components_it->first == tc_interface_type)
                        ++components_it;
                    Components.erase(components_it);
                    delete ptc_base;
                    it->second = 0;
                }
                it = TCsToBeRemoved.erase(it);
            } else
                ++it;
        }
    }
    return false;
}
