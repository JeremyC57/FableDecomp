// CNavQuadTree::SetUpRegions(long mapIndex) -- draft
// Leaf: ?SetUpRegions@CNavQuadTree@@AAEXJ@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"

class CNavRegionConnection;
class CNavRegion {
public:
    long RegionID;                                                      // +0
    std::list<CCountedPointer<CNavRegionConnection> > Connections;     // +4
    long MostRecentSearchID;                                            // +8
    CNavRegion(long mapIndex, long internalIndex);
};

CNavRegion::CNavRegion(long mapIndex, long internalIndex)
{
    RegionID = (mapIndex << 16) | (internalIndex & 0xFFFF);
    MostRecentSearchID = 0;
}

class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();
    virtual void GetNeighbours1() const;
    virtual void GetNeighbours2() const;
    virtual bool HasNeighbours() const = 0;
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;
    virtual long GetRegionID() const = 0;
    virtual long GetNodeType() const = 0;
    virtual const CNavRegion* GetRegion() const;
    virtual unsigned char GetPreferabilityCost() const;
    virtual bool IsSwitchable() const;
    virtual bool IsSwitchableOn(unsigned __int64 id) const;
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const = 0;
    virtual bool MakeRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& nodes) = 0;
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& nodes) = 0;
    virtual bool RemoveRegionZero() = 0;
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
public:
    virtual bool HasNeighbours() const;
    virtual long GetRegionID() const;
    virtual long GetNodeType() const;
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;
    virtual bool MakeRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& nodes);
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& nodes);
    virtual bool RemoveRegionZero();
};

class CNavigationPosition { public: C2DVector Position; long Layer; };
class C2DLineF;

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();
};

class CNavQuadTree : public CBaseClassNonCopyable {
public:
    class CNavigationLayer {
    public:
        CArray<CANavQuadTreeNode*> Nodes;                                   // +0x00
        std::vector<std::list<C2DLineF> > LineLists;                        // +0x0c
        unsigned char SwitchableLineLists[0x10];                            // +0x18
    };
    bool          Initialised;                     // +0x04
    bool          Activated;                       // +0x05
    bool          Initialising;                    // +0x06
    C2DVector     MapPos;                          // +0x07
    float         MapWidth;                        // +0x10
    float         MapHeight;                       // +0x14
    long          MapIndex;                        // +0x18
    std::vector<CNavigationLayer> NavigationLayers;   // +0x1c
    unsigned char TransitionPoints[0xc];           // +0x28
    std::vector<unsigned char> TopologyWeights;    // +0x34
    unsigned char SwitchableNodes[0x10];           // +0x40
    std::vector<CNavigationPosition> ActionPoints; // +0x50
    unsigned char DetailedAreas[0xc];              // +0x5c
    std::vector<CNavigationPosition> RegionSeeds;  // +0x68
    std::list<CNavRegion> Regions;                 // +0x74
    static CNavBlockedQuadTreeNode BlockedNode;
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& pos, long layer) const;    // 0x00A770B0
private:
    void SetUpRegions(long mapIndex);
};

void CNavQuadTree::SetUpRegions(long mapIndex)
{
    long regionIndex = 1;
    Regions.push_back(CNavRegion(mapIndex, regionIndex));
    for (std::vector<CNavigationPosition>::iterator seed = RegionSeeds.begin(); seed != RegionSeeds.end(); ++seed) {
        CANavQuadTreeNode* node = const_cast<CANavQuadTreeNode*>(GetNodeAt(seed->Position, seed->Layer));
        if (node) {
            std::list<CANavQuadTreeNode*> nodes;
            if (node->MakeRegion(&Regions.back(), nodes)) {
                for (std::list<CANavQuadTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
                    (*it)->MakeRegion(&Regions.back(), nodes);
                ++regionIndex;
                Regions.push_back(CNavRegion(mapIndex, regionIndex));
            }
        }
    }
    unsigned long layer;
    for (layer = 0; layer < NavigationLayers.size(); ++layer) {
        for (unsigned long i = 0; i < NavigationLayers[layer].Nodes.size(); ++i) {
            CANavQuadTreeNode* node = NavigationLayers[layer].Nodes[i];
            std::list<CANavQuadTreeNode*> nodes;
            while (node->AddNavigableNodeNeighbouringSwitchableNodeToRegion(&Regions.back(), nodes)) {
                for (std::list<CANavQuadTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
                    (*it)->MakeRegion(&Regions.back(), nodes);
                ++regionIndex;
                Regions.push_back(CNavRegion(mapIndex, regionIndex));
                nodes.clear();
            }
        }
    }
    for (layer = 0; layer < NavigationLayers.size(); ++layer) {
        for (unsigned long i = 0; i < NavigationLayers[layer].Nodes.size(); ++i) {
            CANavQuadTreeNode* node = NavigationLayers[layer].Nodes[i];
            if (node->RemoveRegionZero() && node != &BlockedNode) {
                delete node;
                NavigationLayers[layer].Nodes[i] = &BlockedNode;
            }
        }
    }
}
