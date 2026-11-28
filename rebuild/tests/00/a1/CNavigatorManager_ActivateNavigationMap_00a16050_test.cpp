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
#include <cassert>
#include <cstdio>
#include <cstring>
static int activations,links,worlds,sizes;
static const CNavQuadTree* expectedTree;
static const CNavigatorManager* expectedManager;
static float expectedX,expectedY;
void CNavQuadTree::Activate(){assert(this==expectedTree&&!Activated);Activated=true;++activations;}
void CNavQuadTree::LinkToNeighbouringMaps(const CNavigatorManager& manager){assert(this==expectedTree&&Activated&&&manager==expectedManager);++links;}
float CNavQuadTree::GetSmallestNodeSize(){++sizes;return .5f;}
void CNavigatorManager::OutputWorld(const C2DVector& p,unsigned char layer){
 assert(this==expectedManager&&p.X==expectedX+.5f&&p.Y==expectedY+.5f&&layer==(unsigned char)nav_layer);++worlds;
}
int main(){
 for(int mode=0;mode<64;++mode){
    unsigned long mgr[sizeof(CNavigatorManager)/4]={0}, tree[3][sizeof(CNavQuadTree)/4]={0};
    unsigned long header[2]={0}, nodes[3][10]={0};int count=(mode>>1)%4;int desired=mode%5;
    CNavigatorManager& manager=*reinterpret_cast<CNavigatorManager*>(mgr);expectedManager=&manager;
    mgr[0x24/4]=(unsigned long)header;
    header[0]=count?(unsigned long)nodes[0]:(unsigned long)header;
    header[1]=count?(unsigned long)nodes[count-1]:(unsigned long)header;
    for(int i=0;i<count;++i){
        nodes[i][0]=i+1<count?(unsigned long)nodes[i+1]:(unsigned long)header;
        nodes[i][1]=i?(unsigned long)nodes[i-1]:(unsigned long)header;
        nodes[i][2]=(unsigned long)tree[i];nodes[i][4]=i;nodes[i][5]=i+7;nodes[i][6]=i+19;
        reinterpret_cast<unsigned char*>(tree[i])[5]=(mode&2)!=0;
    }
    bool found=desired<count;expectedTree=found?reinterpret_cast<CNavQuadTree*>(tree[desired]):0;
    expectedX=(float)(desired+7);expectedY=(float)(desired+19);
    output_nav_data=(mode&4)!=0;nav_layer=(unsigned char)(mode*3);
    activations=links=worlds=sizes=0;manager.ActivateNavigationMap(desired);
    assert(activations==(found&&!(mode&2))&&links==activations);
    assert(worlds==(found&&output_nav_data)&&sizes==2*worlds);
 }
 puts("00a16050_TEST PASS");return 0;
}
