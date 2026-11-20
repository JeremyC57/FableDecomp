// CNavigatorAStar::IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const -- true for the same node, false when either is null, else the nodes' own equality (CANavQuadTreeNode::operator== @ 0x00A763F0: both coordinates within 1e-4)
// Leaf: ?IsEqual@CNavigatorAStar@@ABE_NPBVCANavQuadTreeNode@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1A170, length 48 bytes.
// Shape: `this` is not read, no frame, the operator's al returned straight through, `ret 8`.
// NF150: on the shared navigation headers with the macro set of the landed 0x00A1CEB0, whose FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
// branch carries the inline body of the private const ?IsEqual@CNavigatorAStar@@ABE_NPBVCANavQuadTreeNode@@0@Z (Ego_r.pdb / FableWin.pdb).
// Retail expands it in CNavigatorAStar::Initialise 0x00A1CEB0 and calls this out-of-line copy from IsNewDestinationGoingToCauseRestart
// 0x00A1ABF0, so this row is the header inline's pick-any COMDAT: a strong definition here gives LNK2005 against the 0x00A1CEB0 object,
// which emits the same COMDAT (link/strong.log, `LINK=` cleared).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NAV_NODETYPE_ORIGINAL_POLYMORPHISM_PASS23
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_QUEUE_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_API_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
#include "engine/CNavigatorAStar.h"

namespace {
// Fidelity: scaffolding, not retail: names the header's inline (private) IsEqual in an explicit instantiation, where access is not
// checked, so this object emits it out of line as a pick-any COMDAT (retail's 0x00A1A170). Internal to this TU; nothing calls it.
template <bool (CNavigatorAStar::*Member)(const CANavQuadTreeNode*, const CANavQuadTreeNode*) const>
class CIsEqualReference {
public:
    static bool (CNavigatorAStar::*Reference())(const CANavQuadTreeNode*, const CANavQuadTreeNode*) const { return Member; }
};
template class CIsEqualReference<&CNavigatorAStar::IsEqual>;
}
