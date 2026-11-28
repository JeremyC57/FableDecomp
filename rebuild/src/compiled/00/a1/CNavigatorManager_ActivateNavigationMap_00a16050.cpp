// Leaf: ?ActivateNavigationMap@CNavigatorManager@@QAEXJ@Z
// Flags:
#define FABLE_RETAIL_NAV_WIDTH_PASS30
#define FABLE_RETAIL_NAV_ACTIVATE_MAP_PASS30
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_STLP_LIST
#include "engine/CNavigatorManager.h"
#include "engine/CNavQuadTree.h"

static bool output_nav_data = false;
static unsigned char nav_layer = 0;

void CNavigatorManager::ActivateNavigationMap(long mapIndex)
{
    for (std::list<CNavQuadTreeMap>::const_iterator it = QuadTrees.begin(); it != QuadTrees.end(); ++it) {
        if (it->Index == mapIndex) {
            if (!it->PQuadTree->IsActivated()) {
                it->PQuadTree->Activate();
                it->PQuadTree->LinkToNeighbouringMaps(*this);
            }
            if (output_nav_data) {
                C2DVector pos = it->MapArea.GetTLPos().ToC2DCoordF() +
                                C2DVector(CNavQuadTree::GetSmallestNodeSize(), CNavQuadTree::GetSmallestNodeSize());
                OutputWorld(pos, nav_layer);
            }
            return;
        }
    }
}