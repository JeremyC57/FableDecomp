// CNavQuadTree::SaveToFile(CAFile& file) const -- write the navigation quad tree to a file: a size placeholder, the format version,
//   the map size, the region and action-point counts, every action point relative to the map origin, the layer count, the node
//   index table (the static blocked node is index 0, then each layer's nodes number themselves), every node, the end marker, and
//   finally the record size patched into the placeholder.
// Leaf: ?SaveToFile@CNavQuadTree@@QBEXAAVCAFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78B20, 678 bytes, thiscall `ret 4`, esp frame 0x28. Identity: Ego_r.pdb / FableWin.pdb
//   ?SaveToFile@CNavQuadTree@@QBEXAAVCAFile@@@Z; FableWin 0x03289B50 (debug) has the same statement order through the inline
//   WriteVar<long / float / unsigned int> templates, make_pair(&BlockedNode, 0) and the CSaveNavigationPositionToFile(file, MapPos) functor.
// Callees: CAFile slots 4 Write (+0x10) / 5 SetPosition (+0x14) / 7 GetPosition (+0x1C); std::for_each<const CNavigationPosition*,
//   CSaveNavigationPositionToFile> 0x00A7E440 (the /Gr __fastcall COMDAT, landed); malloc 0x00BFEA0E / free 0x00BFEA14 (the map
//   header); _Rb_tree<const CANavQuadTreeNode*, ..>::insert_unique 0x00A819B0; _M_erase 0x00A7E300; CANavQuadTreeNode slots 20
//   AddToIndexTable (+0x50) / 21 SaveToFile (+0x54). Data: CNavQuadTree::BlockedNode 0x013D2D2C, the version 8 at 0x0129DCC8 and
//   the end marker 0xACBDEF12 at 0x0129DCCC (TU-static consts; no symbol in either PDB, names descriptive).
// Shape: the for_each instance is the shared fable_algorithm.h FABLE_FASTCALL declaration (retail COMDAT 0x00A7E440, /Gr there); no TU flag.
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_SAVETOFILE
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SAVETOFILE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS
#define FABLE_RETAIL_CAFILE_WRITEVAR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_algorithm.h"
#include "engine/CAFile.h"
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/CNavigationPosition.h"

static const long NAV_QUAD_TREE_FILE_VERSION = 8;                    // 0x0129DCC8 (descriptive)
static const long NAV_QUAD_TREE_FILE_END_MARKER = 0xACBDEF12;        // 0x0129DCCC (descriptive)

class CSaveNavigationPositionToFile {                                // FableWin.pdb ??0CSaveNavigationPositionToFile@@QAE@AAVCAFile@@ABVC2DVector@@@Z
public:
    CSaveNavigationPositionToFile(CAFile& file, const C2DVector& mapPos) : File(&file), MapPos(mapPos) { }
    void operator()(const CNavigationPosition& pos) const            // FableWin.pdb ??RCSaveNavigationPositionToFile@@QBEXABVCNavigationPosition@@@Z
    {
        CNavigationPosition relative(pos.Position - MapPos, pos.Layer);
        File->WriteVar(relative);
    }
private:
    CAFile*   File;                                                  // +0x00
    C2DVector MapPos;                                                // +0x04
};

void CNavQuadTree::SaveToFile(CAFile& file) const
{
    long startPos = file.GetPosition();
    long size = 0;
    file.WriteVar(size);
    file.WriteVar(NAV_QUAD_TREE_FILE_VERSION);
    file.WriteVar(MapWidth);
    file.WriteVar(MapHeight);
    file.WriteVar(Regions.size());
    file.WriteVar(ActionPoints.size());
    std::for_each(ActionPoints.begin(), ActionPoints.end(), CSaveNavigationPositionToFile(file, MapPos));
    file.WriteVar(NavigationLayers.size());
    std::map<const CANavQuadTreeNode*, long> nodeIndexTable;
    nodeIndexTable.insert(std::make_pair(&BlockedNode, 0));
    long index = 1;
    std::vector<CNavigationLayer>::const_iterator it;
    for (it = NavigationLayers.begin(); it != NavigationLayers.end(); ++it) {
        const CNavigationLayer& layer = *it;
        for (std::vector<CANavQuadTreeNode*>::const_iterator node = layer.Nodes.begin(); node != layer.Nodes.end(); ++node)
            (*node)->AddToIndexTable(index, nodeIndexTable);
    }
    file.WriteVar(index - 1);
    for (it = NavigationLayers.begin(); it != NavigationLayers.end(); ++it) {
        const CNavigationLayer& layer = *it;
        for (std::vector<CANavQuadTreeNode*>::const_iterator node = layer.Nodes.begin(); node != layer.Nodes.end(); ++node)
            (*node)->SaveToFile(file, nodeIndexTable, true, MapPos);
    }
    file.WriteVar(NAV_QUAD_TREE_FILE_END_MARKER);
    size = file.GetPosition();
    file.SetPosition(startPos);
    file.WriteVar(size);
    file.SetPosition(size);
}