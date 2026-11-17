// CTCContainerRewardHero::ReleaseItems(float speed) -- pop every reward item out of the container: each item is created as a thing object a little above the container (0.1 up) and launched along a popout vector laid out in rings of up to eight, then every created object is told to ignore its siblings and the item list is cleared
// Leaf: ?ReleaseItems@CTCContainerRewardHero@@IAEXM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E6E0, 384 bytes (`ret 4`).
// Callees: CThing::GetPos() const @ 0x004C73D0, GeneratePopoutVectorFromVars(long, long, long, float) const @ 0x0074E140
// (C3DVector by hidden pointer), CreateNewObject(long, const C3DVector&, const C3DVector&, const CCharString&) @ 0x0074E560,
// vector<CThingObject*>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x0074F900, std::for_each<CThingObject**, functor>
// @ 0x0074FBB0 (the functor returned by hidden pointer; ecx = return slot, edx = first: modelled as __fastcall),
// vector<pair<long, CCharString>>::erase(first, last) @ 0x0074FB20, free @ 0x00BFEA14 (the local vector's storage).
// Globals: 0.1f @ 0x0122EE58.
// Layout: Ego_r.pdb CTCContainerRewardHero (names from the PDB symbols ?ReleaseItems@...@@IAEXM@Z etc.); retail ContainerItems
// (STLport vector<pair<long, CCharString>>, 8-byte elements) at +0x0c as the landed HasReward 0x0074E500.
// Shape: the per-ring count is min(size, 8) written as a preset 8; the local vector is pushed inline (null-tested placement
// store, overflow call with the tag temporary in the dead popout-return slot); the functor's return slot is the dead `speed`
// argument slot.
// Name-fix (agentNF55, round 7): the local CTCContainerRewardHero / CContainerItemVector / CThingObjectVector models replaced by the
//   shared engine/CTCContainerRewardHero.h (: CTCContainer : retail CTCBase; opt-ins GETREWARDITEMS for the private CreateNewObject and
//   NF55's RELEASEITEMS for this protected method) and the STLport vectors: ContainerItems.clear() calls the out-of-line
//   vector<pair<long, CCharString> >::erase 0x0074FB20 (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE), push_back expands with the
//   visible out-of-line vector<CThingObject*>::_M_insert_overflow 0x0074F900 (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), and the
//   functor goes to the STLport for_each declaration (stlport/fable_algorithm.h) as the landed 0x0074FBB0 instance
//   ??$for_each@PAPAVCThingObject@@VCAddObjectsNotSolidTo@@@std@@YI... (Ego_r.pdb names the TU-local functor CAddObjectsNotSolidTo).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTCCONTAINERREWARDHERO_GETREWARDITEMS
#define FABLE_RETAIL_CTCCONTAINERREWARDHERO_RELEASEITEMS
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "stlport/fable_algorithm.h"
#include "engine/CTCContainerRewardHero.h"

class CThingObject;
class CAddObjectsNotSolidTo {                           // TU-local functor: each object's CTCSolidToThing ignores the others
public:
    const std::vector<CThingObject*>* ObjectList;       // +0x00
    CAddObjectsNotSolidTo(const std::vector<CThingObject*>& objects) : ObjectList(&objects) { }
    void operator()(CThingObject* object);
};

void CTCContainerRewardHero::ReleaseItems(float speed)
{
    if (ContainerItems.empty())
        return;
    const C3DVector& position = GetParentThing().GetPos();
    C3DVector pos;
    pos.X = position.X;
    pos.Y = position.Y;
    pos.Z = position.Z + 0.1f;
    long count = ContainerItems.size();
    C3DVector velocity(0.0f, 0.0f, 0.0f);
    long perRing = 8;
    if (count < 8)
        perRing = count;
    long index = 0;
    long ring = 0;
    std::vector<CThingObject*> objects;
    for (std::vector<std::pair<long, CCharString> >::iterator it = ContainerItems.begin(); it != ContainerItems.end(); ++it) {
        velocity = GeneratePopoutVectorFromVars(index, perRing, ring, speed);
        if (++index == perRing) {
            index = 0;
            ++ring;
        }
        CThingObject* object = CreateNewObject(it->first, pos, velocity, it->second);
        objects.push_back(object);
    }
    std::for_each(objects.begin(), objects.end(), CAddObjectsNotSolidTo(objects));
    ContainerItems.clear();
}
