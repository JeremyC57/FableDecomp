// CNavQuadTree::IsAreaClear(const CANavQuadTreeNode*, bool&, vector<unsigned __int64>&) const -- draft
// Leaf: ?IsAreaClear@CNavQuadTree@@QBE_NPBVCANavQuadTreeNode@@AA_NAAV?$vector@_KV?$allocator@_K@std@@@std@@@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CVectorMap.h"
#include "engine/GFFloatToLongFloor.h"

extern const float g_QuadTreeLevelSizes[];         // 0x0129DCAC

class C2DLineF { public: C2DVector Point1; C2DVector Point2; };

class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    float GetSize() const { return g_QuadTreeLevelSizes[QuadTreeLevel]; }
    C2DBoxF GetAreaCovering() const
    {
        float half = GetSize() * 0.5f;
        return C2DBoxF(Position.X - half, Position.Y - half, half + Position.X, half + Position.Y);
    }
};

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
        CVectorMap<unsigned __int64, std::list<C2DLineF> > SwitchableLineLists;   // +0x18
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
    bool IsAreaClear(const CANavQuadTreeNode* node, bool& switchable, std::vector<unsigned __int64>& ids) const;
private:
    bool IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lines) const;   // 0x00A76F30
};

// STLport _vector.c growth paths: out of line in retail (0x00A81590 for the line-list pointers, 0x00617370 for the ids).
#pragma auto_inline(off)
template<> void std::vector<const std::list<C2DLineF>*>::_M_insert_overflow(iterator position, const value_type& x, const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    value_type* newStart=this->_M_end_of_storage.allocate(len);
    value_type* newFinish=(value_type*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(value_type*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<> void std::vector<unsigned __int64>::_M_insert_overflow(iterator position, const value_type& x, const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    value_type* newStart=this->_M_end_of_storage.allocate(len);
    value_type* newFinish=(value_type*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(value_type*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#pragma auto_inline(on)

bool CNavQuadTree::IsAreaClear(const CANavQuadTreeNode* node, bool& switchable, std::vector<unsigned __int64>& ids) const
{
    switchable = false;
    C2DBoxF area = node->GetAreaCovering();
    if (node->NavLayer == 0 && !TopologyWeights.empty()) {
        for (long x = (long)(area.TLX - MapPos.X); x < area.BRX - MapPos.X; ++x)
            for (long y = (long)(area.TLY - MapPos.Y); y < area.BRY - MapPos.Y; ++y)
                if (TopologyWeights[(long)(y * MapWidth + x)] == 255)
                    return false;
    }
    std::vector<const std::list<C2DLineF>*> lines;
    const std::vector<std::list<C2DLineF> >& lineLists = NavigationLayers[node->NavLayer].LineLists;
    if (!lineLists.empty()) {
        C2DVector relPos = node->Position - MapPos;
        long minX, minY, maxX, maxY;
        minX = GFFloatToLongFloor((relPos.X - node->GetSize() * 0.5f) / 8.0f);
        minY = GFFloatToLongFloor((relPos.Y - node->GetSize() * 0.5f) / 8.0f);
        maxX = GFFloatToLongFloor((relPos.X + node->GetSize() * 0.5f) / 8.0f);
        maxY = GFFloatToLongFloor((relPos.Y + node->GetSize() * 0.5f) / 8.0f);
        if (maxX == minX) ++maxX;
        if (maxY == minY) ++maxY;
        for (unsigned long x = minX; x < (unsigned long)maxX; ++x)
            for (unsigned long y = minY; y < (unsigned long)maxY; ++y)
                lines.push_back(&lineLists[(long)(x + y * MapWidth / 8.0f)]);
    }
    if (IsAreaBlockedByLines(area, lines))
        return false;
    const CVectorMap<unsigned __int64, std::list<C2DLineF> >& switchables = NavigationLayers[node->NavLayer].SwitchableLineLists;
    for (CVectorMap<unsigned __int64, std::list<C2DLineF> >::const_iterator it = switchables.begin(); it != switchables.end(); ++it) {
        std::vector<const std::list<C2DLineF>*> switchLines;
        switchLines.push_back(&it->second);
        if (IsAreaBlockedByLines(area, switchLines)) {
            switchable = true;
            ids.push_back(it->first);
        }
    }
    return !switchable;
}
