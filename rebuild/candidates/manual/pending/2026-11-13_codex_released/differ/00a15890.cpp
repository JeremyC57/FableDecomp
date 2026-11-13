// CNavigatorManager::OutputWorld(const C2DVector& position, unsigned char navLayer) -- dump the navigation world reachable from the node under a position to nav.data (the FablePathVisualiser file, else nav.data in the working folder): a "0.0, 0.0, 0.0, 0.0" header, then for each node its covering square's four corners and a type (4 switchable, 0 / 2 / 3 by preferability cost, 1 blocked, 5 a blocked edge cell); a flood fill over GetNextNeighbour with an open stack, a visited-position list and copies of the shared blocked node, which are deleted at the end
// Grade: DIFFER(1983v1973) -- flood fill, branch order, out-of-line GetAreaCovering at all seven sites and the in-loop push_back expansions are retail-shaped; residue: VC7.1 inlines the first pre-loop open.push_back (simplified against the known-empty vector) where retail calls 0x00A17400 out of line at all three pre-loop sites, the static type helper is called out of line where retail expands it, the frame is 0x94 vs 0x8C, and retail keeps 0 in ebx (cmp reg, ebx) and the file in edi while VC7.1 uses test and spills the file
// Next idea: find the inliner shape that leaves every pre-loop push_back out of line but expands the in-loop ones (retail's inline budget: perhaps the pre-loop branches as a separate inline helper, or the output as a macro so the type test is expanded), then re-check the zero register
// Leaf: ?OutputWorld@CNavigatorManager@@QAEXABVC2DVector@@E@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A15890, 1973 bytes, thiscall `ret 8`, 8-aligned ebp frame (/O2).
// Identity: Ego_r.pdb ?OutputWorld@CNavigatorManager@@QAEXABVC2DVector@@E@Z (donor order; chunk 783 NOTES). The catalog label
//   InitializeNavigationData is wrong.
// Callees: CNavigatorManager::GetNodeAt 0x00A14F70 / GetNextNeighbour 0x00A15140, CANavQuadTreeNode::GetAreaCovering 0x00A763B0
//   (out of line here), slots 8 GetPreferabilityCost (+0x20) / 9 IsSwitchable (+0x24), CNavBlockedQuadTreeNode copy ctor 0x00A76900,
//   operator new 0x00BFEA1A, free 0x00BFEA14, fopen 0x00BFEDFA, fprintf 0x00BFEDF4, fclose 0x00BFEA20, vector push_back 0x00A17400
//   (const CANavQuadTreeNode*) / 0x00A17440 (CNavBlockedQuadTreeNode*), _M_insert_overflow 0x00A17190 / 0x00A17270 / 0x004416E0
//   (C2DVector), std::__find<C2DVector*> 0x00A169B0, std::__find_if<..., IsSameNavQuadTreeNode> 0x00A16550.
// Data: "wt" 0x0129CB3C, the FablePathVisualiser path 0x0129CAD8, "nav.data" 0x0129CACC, the header 0x0129CAB8, the node format
//   0x0129CA94.
#include <stdio.h>
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_AREA_OUTOFLINE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE_COPY
#include "engine/CNavBlockedQuadTreeNode.h"
#define CNavigatorManager CNavigatorManagerLayout
#include "engine/CNavigatorManager.h"
#undef CNavigatorManager

class IsSameNavQuadTreeNode { public:
    const CANavQuadTreeNode* Node;
    IsSameNavQuadTreeNode(const CANavQuadTreeNode* node) : Node(node) { }
    bool operator()(const CANavQuadTreeNode* node) const { return *Node == *node; }
};
namespace std {
template<class I, class T> I __fastcall __find(I first, I last, const T& val, const random_access_iterator_tag&);
template<class I, class P> I __fastcall __find_if(I first, I last, P pred, const random_access_iterator_tag&);
template<class I, class T> inline I find(I first, I last, const T& val) { return __find(first, last, val, random_access_iterator_tag()); }
template<class I, class P> inline I find_if(I first, I last, P pred) { return __find_if(first, last, pred, random_access_iterator_tag()); }
}

class CNavigatorManager : public CNavigatorManagerLayout { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char navLayer) const;   // 0x00A14F70
    const CANavQuadTreeNode* GetNextNeighbour(const CANavQuadTreeNode& node, const CANavQuadTreeNode* previous,
                                              const C2DVector& position, bool* blocked);           // 0x00A15140
    void OutputWorld(const C2DVector& position, unsigned char navLayer);
};

static inline int GetOutputType(const CANavQuadTreeNode* node)
{
    unsigned char cost = node->GetPreferabilityCost();
    if (cost == 0)
        return 0;
    if (cost == 0x80)
        return 2;
    return node->GetPreferabilityCost() ? 3 : 3;
}

void CNavigatorManager::OutputWorld(const C2DVector& position, unsigned char navLayer)
{
    std::vector<const CANavQuadTreeNode*> open;
    std::vector<C2DVector> visited;
    std::vector<CNavBlockedQuadTreeNode*> blockedCopies;
    const CANavQuadTreeNode* node = GetNodeAt(position, navLayer);
    if (!node)
        return;
    FILE* file = fopen("C:\\Documents and Settings\\mducker\\My Documents\\Visual Studio Projects\\FablePathVisualiser\\nav.data", "wt");
    if (!file) {
        file = fopen("nav.data", "wt");
        if (!file)
            return;
    }
    fprintf(file, "0.0, 0.0, 0.0, 0.0\n");
    if (node->IsSwitchable()) {
        C2DBoxF area = node->GetAreaCovering();
        fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX, area.BRY, 4);
        open.push_back(node);
    } else if (!node->Blocked) {
        C2DBoxF area = node->GetAreaCovering();
        fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX, area.BRY,
                GetOutputType(node));
        open.push_back(node);
    } else {
        C2DBoxF area = node->GetAreaCovering();
        fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX, area.BRY, 1);
        CNavBlockedQuadTreeNode* copy = new CNavBlockedQuadTreeNode(*(const CNavBlockedQuadTreeNode*)node);
        node = copy;
        blockedCopies.push_back(copy);
        open.push_back(copy);
    }
    bool first;
    bool blocked;
    C2DVector start;
    while (open.size()) {
        const CANavQuadTreeNode* current = open.back();
        visited.push_back(current->Position);
        open.pop_back();
        first = true;
        blocked = false;
        const CANavQuadTreeNode* neighbour = GetNextNeighbour(*current, 0, start, &blocked);
        while (neighbour) {
            if (first) {
                first = false;
                start = neighbour->Position;
            }
            if (neighbour->Blocked && !neighbour->IsSwitchable()) {
                CNavBlockedQuadTreeNode* copy = new CNavBlockedQuadTreeNode(*(const CNavBlockedQuadTreeNode*)neighbour);
                neighbour = copy;
                blockedCopies.push_back(copy);
            }
            if (std::find(visited.begin(), visited.end(), neighbour->Position) == visited.end()
                && std::find_if(open.begin(), open.end(), IsSameNavQuadTreeNode(neighbour)) == open.end()) {
                if (neighbour->IsSwitchable()) {
                    C2DBoxF area = neighbour->GetAreaCovering();
                    fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX,
                            area.BRY, 4);
                    open.push_back(neighbour);
                } else if (!neighbour->Blocked) {
                    C2DBoxF area = neighbour->GetAreaCovering();
                    fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX,
                            area.BRY, GetOutputType(neighbour));
                    open.push_back(neighbour);
                } else if (blocked) {
                    C2DBoxF area = neighbour->GetAreaCovering();
                    fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX,
                            area.BRY, 5);
                    visited.push_back(neighbour->Position);
                } else {
                    C2DBoxF area = neighbour->GetAreaCovering();
                    fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", area.TLX, area.TLY, area.BRX, area.TLY, area.BRX, area.BRY, area.TLX,
                            area.BRY, 1);
                    open.push_back(neighbour);
                }
            }
            neighbour = GetNextNeighbour(*current, neighbour, start, &blocked);
        }
    }
    if (file)
        fclose(file);
    while (blockedCopies.size()) {
        CNavBlockedQuadTreeNode* copy = blockedCopies.back();
        blockedCopies.pop_back();
        delete copy;
    }
}
