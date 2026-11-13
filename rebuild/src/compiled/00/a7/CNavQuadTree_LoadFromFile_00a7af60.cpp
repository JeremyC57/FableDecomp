// CNavQuadTree::LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess& process) -- load a navigation map: regions, action points, layers of quad-tree nodes (index table resolved after loading), switchable nodes sorted and unblocked
// Leaf: ?LoadFromFile@CNavQuadTree@@QAE_NAAVCAFile@@JABVC2DBoxI@@AAVCASuspendableProcess@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7AF60, 2503 bytes, thiscall `ret 10h`, frame `and esp,-8 ; sub esp,0E4h` (the double
//   breath time aligns the frame).
// Flags evidence: (1) measured -- without /Gr the STLport helpers this TU reaches undecorated (__destroy_aux 0x00A85930 / 0x00A83420 /
//   0x00A827D0, `mov ecx ; mov edx ; push` at 0x00A7B292..0x00A7B304) turn into cdecl pushes, 2519v2503 (hand-back grades.log 24); (2) the landed
//   same-TU siblings 0x00A85870 __copy and 0x00A762A0 GFProcessBreath are /Gr fastcall bodies.
// Identity: Ego_r.pdb + FableWin.pdb ?LoadFromFile@CNavQuadTree@@QAE_NAAVCAFile@@JABVC2DBoxI@@AAVCASuspendableProcess@@@Z.
// Callees (Ego_r.pdb / FableWin.pdb spellings unless noted): CCachedFile(long) 0x00A5B590, OpenFromFile 0x00A5B210, Read 0x00A5B370,
//   ~CCachedFile 0x00826380 (called on the version exit, expanded at the end: Close 0x00A5AFA0, ~CWideString 0x0099B510,
//   ~CBaseClassNonCopyable 0x0099A300); CAFile::ReadVar<int> 0x00516180; GFGetTime 0x009E1BC0 and CASuspendableProcess slot 4
//   SuspendableProcess (the five expanded GFProcessBreath copies; landed 0x00A762A0, same text); CNavRegion copy ctor 0x00A83110;
//   CNavSwitchableLeafNode / CNavNavigableLeafNode / CNavQuadTreeNode file ctors 0x00A79A30 / 0x00A772D0 / 0x00A76580;
//   CANavQuadTreeNode slot 22 ConvertIndexesToPointers; CNavSwitchableLeafNode::SetAsBlocked 0x00A78800; the static
//   CNavQuadTree::BlockedNode 0x013D2D2C. STLport 4.6 (source names; landed rows bind them): list<CCountedPointer<
//   CNavRegionConnection> >::clear 0x00A80A20, vector<CNavigationPosition>::reserve 0x00A80980 / _M_insert_overflow 0x00517BA0,
//   map<long, CANavQuadTreeNode*> insert_unique 0x00A81A70 / _M_erase 0x00A7E340, vector<CNavigationLayer> __copy 0x00A85870 /
//   __destroy_aux 0x00A85930 / _M_fill_insert 0x00A85CC0, the default layer's destroy helpers 0x00A83420 / 0x00A827D0,
//   vector<CANavQuadTreeNode*>::_M_fill_insert 0x00A81890, vector<pair<unsigned __int64, CANavQuadTreeNode*> >::_M_insert_overflow
//   0x00A82480, std::sort over that pair 0x00A841F0; malloc 0x00BFEA0E, free 0x00BFEA14, memmove 0x00BFEAE6, operator new 0x00BFEA1A,
//   operator delete 0x00BFE9BC, _ftol2 0x00BFEA70. Constants: 0.005 (double, 0x0129DD30), 1/32 (float, 0x0129DD98).
// File format (retail order of the Read calls): int file size (ReadVar), long version (below 5: fail), float MapWidth, float
//   MapHeight, region count, action point count + 12-byte CNavigationPosition records (map-relative), layer count, node count, then
//   per node: [switchable flag, version 6+] top-level flag, blocked flag; unless a plain blocked node: leaf flag, quad-tree level,
//   layer, 8-byte position, long index, then the node's own data read by its constructor (version 7+: with preferability); a long
//   end marker closes the list.
// Shape notes (what the bytes decide):
//   - MapPos = area.GetTLPos().ToC2DCoordF(): two fild / fstp pairs through C2DCoordI (0x00A7AF78..0x00A7AFA7).
//   - Region loop (0x00A7B043..0x00A7B0DE): ONE guard `xor edi,edi ; test eax,eax ; jbe`, the temporary CNavRegion built in place
//     (MostRecentSearchID stored before RegionID at 0x00A7B07D / 0x00A7B085) and the copy constructor 0x00A83110 CALLED from the
//     inline push_back. The counter is initialised ahead of an `if (no_regions != 0)` guard: with the `if` the copy site sits at
//     static frequency 0.625 (c2 inline trace: copy ctor c1 76, score 82 >= 74, called) and NavigationLayers.resize after the loop
//     still expands; a plain `for` (frequency 1.25, score 70) expands the copy and the frame grows to 0xF4, a do-while stops the
//     resize expanding; with the counter set inside the `for` the guard is tested twice (hand-back grades.log 8, 9, 11, 12, 14, 29, 30).
//   - The first map entry (index 0 -> &BlockedNode) and every node insert build an unnamed value_type temporary.
//   - `(MapWidth / 32.0f) * (MapHeight / 32.0f)` is the two fmul by 1/32 at 0x00A7B333..0x00A7B345 (height loaded first).
//   - A plain blocked node (not switchable) is the shared BlockedNode with its NavLayer set to the current layer (0x00A7B4BD).
//   - Switchable leaves are entered in SwitchableNodes under each of their unique ids (CVectorMap::Insert expanded: push_back + the
//     Dirty byte at +0xD); top-level nodes fill the layer tables in file order, `node_in_layer` wrapping into the next layer.
//   - After the end marker every top-level node resolves its indices through slot 22; SwitchableNodes.Update() sorts the dirty
//     map (0x00A7B7DA..0x00A7B7F2) and every switchable node is opened (SetAsBlocked(id, false)); Initialised = true.
// Helper texts: FABLE_RETAIL_STLP_VECTOR_ASSIGN declares STLport's out-of-line vector::operator=, so CNavigationLayer's implicit
//   operator= is the three member vector assignments (0x00A80EE0 / 0x00A83D70 / 0x00A84940) plus the Dirty byte, not a bitwise
//   copy. The TU's own std::__copy<CNavigationLayer*, CNavigationLayer*, int> then equals retail 0x00A85870 (97 bytes) and c2
//   keeps it a call by itself (c2 inline trace of this TU: size 97, cost 80, score 137 in __copy_ptrs / 103 at the row's site,
//   threshold 74), while erase (cost 50, score 37) and resize (cost 95, score 67) expand: retail's `call 0x00A85870` at 0x00A7B28D
//   inside the expanded erase. With the bitwise operator= the copy scored 72, expanded into erase and erase was called (frame
//   0xF4, hand-back grades.log 20). No inline decision of the row is forced. Free model check (hand-back NOTES.md): 39 other functions this TU emits
//   equal their retail copies instruction for instruction (0x00A78680, 0x00A762A0, 0x00A83110, 0x00826380, 0x00A80A20, 0x00A7E340,
//   0x00A81A70, 0x00A85EC0, 0x00A85F30, 0x00A85870, 0x00A85930, 0x00A85420, 0x00A85560, 0x00A85810, 0x00A85B90, 0x00A851E0 ...).
// Fitted v proven: the `{ i = 0; if ; for }` region loop and the CNavRegion(long, long) text (`: MostRecentSearchID(0)`, RegionID in
//   the body) are FITTED (the bytes prove one guard, the called copy and the store order; a body with the zero first matches too,
//   and both texts still give the landed 0x00A78680 bytes); GetTLPos().ToC2DCoordF() is FITTED over PDB inlines; the rest follows
//   the Read order and the member layout (PROVEN by the bytes). Local names are descriptive.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_LOADSLOTS
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CCachedFile.h"
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/CNavQuadTreeNode.h"
#include "engine/CNavSwitchableLeafNode.h"
#include "engine/CASuspendableProcess.h"
#include "engine/C2DBoxI.h"
#include "engine/GFGetTime.h"

// The same-TU body of the landed 0x00A78680 (??0CNavRegion@@QAE@JJ@Z; expanded in the region loop below). The init-list zero gives
// the row's store order; this text still compiles to the 0x00A78680 bytes (graded from this TU: RELOCATION_MATCH).
CNavRegion::CNavRegion(long mapIndex, long internalIndex)
    : MostRecentSearchID(0)
{
    RegionID = (mapIndex << 16) ^ (internalIndex & 0xFFFF);
}

// The same-TU body of the landed 0x00A762A0 (?GFProcessBreath@@YIXAAVCASuspendableProcess@@AAN@Z, Ego_r.pdb + FableWin.pdb; fastcall
// through /Gr), expanded five times in the row.
void GFProcessBreath(CASuspendableProcess& process, double& lastBreathTime)
{
    double now = GFGetTime();
    if (lastBreathTime - now >= 0.005) {
        process.SuspendableProcess();
        lastBreathTime = now;
    }
}

bool CNavQuadTree::LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess& process)
{
    MapIndex = mapIndex;
    MapPos = area.GetTLPos().ToC2DCoordF();
    CCachedFile cached_file(307200);
    cached_file.OpenFromFile(file, &process);
    int file_size;
    cached_file.ReadVar(file_size, &process);
    long version;
    cached_file.Read(&version, 4, NULL);
    if (version < 5)
        return false;
    double last_breath_time = GFGetTime();
    cached_file.Read(&MapWidth, 4, NULL);
    cached_file.Read(&MapHeight, 4, NULL);
    unsigned long no_regions;
    cached_file.Read(&no_regions, 4, NULL);
    {
        unsigned long i = 0;
        if (no_regions != 0) {
            for (; i < no_regions; ++i)
                Regions.push_back(CNavRegion(MapIndex, i + 1));
        }
    }
    unsigned long no_action_points;
    cached_file.Read(&no_action_points, 4, NULL);
    ActionPoints.reserve(no_action_points);
    for (unsigned long i = 0; i < no_action_points; ++i) {
        GFProcessBreath(process, last_breath_time);
        CNavigationPosition action_point;
        cached_file.Read(&action_point, sizeof(action_point), NULL);
        action_point.Position += MapPos;
        ActionPoints.push_back(action_point);
    }
    std::map<long, CANavQuadTreeNode*> nodes;
    nodes.insert(std::map<long, CANavQuadTreeNode*>::value_type(0, &BlockedNode));
    unsigned long no_layers;
    cached_file.Read(&no_layers, 4, NULL);
    NavigationLayers.resize(no_layers);
    long no_nodes_per_layer = (long)((MapWidth / 32.0f) * (MapHeight / 32.0f));
    for (unsigned long i = 0; i < no_layers; ++i) {
        GFProcessBreath(process, last_breath_time);
        NavigationLayers[i].Nodes.resize(no_nodes_per_layer);
    }
    long no_nodes;
    cached_file.Read(&no_nodes, 4, NULL);
    long layer = 0;
    long node_in_layer = 0;
    for (long i = 0; i < no_nodes; ++i) {
        GFProcessBreath(process, last_breath_time);
        bool switchable = false;
        if (version >= 6)
            cached_file.Read(&switchable, 1, NULL);
        bool top_level;
        cached_file.Read(&top_level, 1, NULL);
        bool blocked;
        cached_file.Read(&blocked, 1, NULL);
        CANavQuadTreeNode* node;
        if (!switchable && blocked) {
            BlockedNode.NavLayer = (unsigned char)layer;
            node = &BlockedNode;
        } else {
            bool leaf_node;
            cached_file.Read(&leaf_node, 1, NULL);
            unsigned char quad_tree_level;
            cached_file.Read(&quad_tree_level, 1, NULL);
            unsigned char nav_layer;
            cached_file.Read(&nav_layer, 1, NULL);
            C2DVector position;
            cached_file.Read(&position, 8, NULL);
            C2DVector map_position = position + MapPos;
            long index;
            cached_file.Read(&index, 4, NULL);
            if (leaf_node) {
                if (switchable) {
                    CNavSwitchableLeafNode* switchable_node = new CNavSwitchableLeafNode(map_position, quad_tree_level, nav_layer, cached_file, Regions, version >= 7);
                    for (unsigned long j = 0; j < switchable_node->UniqueIDsOpen.size(); ++j)
                        SwitchableNodes.Insert(switchable_node->UniqueIDsOpen[j].first, switchable_node);
                    node = switchable_node;
                } else
                    node = new CNavNavigableLeafNode(map_position, quad_tree_level, nav_layer, cached_file, Regions, version >= 7);
            } else
                node = new CNavQuadTreeNode(map_position, quad_tree_level, nav_layer, cached_file);
            nodes.insert(std::map<long, CANavQuadTreeNode*>::value_type(index, node));
        }
        if (top_level) {
            NavigationLayers[layer].Nodes[node_in_layer] = node;
            if (++node_in_layer >= no_nodes_per_layer) {
                ++layer;
                node_in_layer = 0;
            }
        }
    }
    long end_marker;
    cached_file.Read(&end_marker, 4, NULL);
    for (std::vector<CNavigationLayer>::iterator it = NavigationLayers.begin(); it != NavigationLayers.end(); ++it) {
        GFProcessBreath(process, last_breath_time);
        for (CANavQuadTreeNode** node_it = it->Nodes.begin(); node_it != it->Nodes.end(); ++node_it)
            (*node_it)->ConvertIndexesToPointers(nodes);
    }
    GFProcessBreath(process, last_breath_time);
    SwitchableNodes.Update();
    for (unsigned long i = 0; i < SwitchableNodes.size(); ++i)
        static_cast<CNavSwitchableLeafNode*>(SwitchableNodes[i].second)->SetAsBlocked(SwitchableNodes[i].first, false);
    Initialised = true;
    return true;
}