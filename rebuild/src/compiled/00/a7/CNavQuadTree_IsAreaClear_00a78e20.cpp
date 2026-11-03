// CNavQuadTree::IsAreaClear(const CANavQuadTreeNode* node, bool& switchable, vector<unsigned __int64>& ids) const -- whether a candidate node's square is free of obstacles: no 255 topology weight under it (layer 0), no navigation line of the cells it covers, and no switchable line list; a switchable list that blocks it sets `switchable` and records its id
// Leaf: ?IsAreaClear@CNavQuadTree@@QBE_NPBVCANavQuadTreeNode@@AA_NAAV?$vector@_KV?$allocator@_K@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78E20, 1042 bytes, thiscall `ret 0Ch`, ebp frame (the inline-asm floor helper).
// Identity: Ego_r.pdb ?IsAreaClear@CNavQuadTree@@QBE_NPBVCANavQuadTreeNode@@AA_NAAV?$vector@_KV?$allocator@_K@std@@@std@@@Z
//   (donor 0x00BBDF70, between AddToIndexTable and CNavRegion::AddConnectionToConnectionList): `this` is the tree (+7 MapPos,
//   +0x10 MapWidth, +0x1C NavigationLayers, +0x34 TopologyWeights), the bool& is cleared first (`mov byte ptr [eax],0`), the
//   u64 ids are pushed on a switchable hit, and the caller CNavQuadTreeNode::Initialise 0x00A79B30 tests the bool result
//   (0x00A79B7C). Label correction: the manifest's `_global::CNavQuadTree_InitializeLeafNode` is propagated.
// Callees: __ftol2 0x00BFEA70, CNavQuadTree::IsAreaBlockedByLines 0x00A76F30 (same TU; body above), the STLport growth paths
//   vector<const list<C2DLineF>*>::_M_insert_overflow 0x00A81590 and vector<unsigned __int64>::_M_insert_overflow 0x00617370,
//   free 0x00BFEA14 (the local vectors' storage). Data: g_QuadTreeLevelSizes 0x0129DCAC, 0.5f, 0.125f (the /8 cell size),
//   2^32 (the unsigned-to-float fix-up).
// Shape: the node's square is the inline GetAreaCovering; the topology scan converts with __ftol2; the cell range of the
//   line lists is GFFloatToLongFloor (the engine's asm floor, engine/GFFloatToLongFloor.h) of the four edges divided by 8,
//   widened to at least one cell, walked with unsigned counters (`jae`, the 2^32 fix-up). VC7.1 keeps the growth paths
//   out of line only when they are same-TU bodies under auto_inline(off) (the header's out-of-class templates are expanded
//   otherwise, 1238 bytes), and keeps the push_back finish pointer in esi only when IsAreaBlockedByLines is a same-TU body
//   (it then knows the vector does not escape; 1043v1042 without it).
// Retail-asm: 0x00A78F80 `fistp dword ptr [ebp-14h]` / 0x00A78FC8 / 0x00A79010 / 0x00A7905E (each with its `fild ; fadd ; fstp ;
//   mov eax ; cmp eax ; jne ; inc dword ptr`): the engine's GFFloatToLongFloor inline-asm block, expanded four times; this row
//   calls the shared helper (engine/GFFloatToLongFloor.h) and has no asm of its own.
// Headers: engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE, engine/CANavQuadTreeNode.h FABLE_RETAIL_CANAVQUADTREENODE (chunk 721
//   + slots 9..15, GetSize / GetAreaCovering), new engine/C2DLineF.h, CNavRegion.h, CNavigationPosition.h.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"
#include "engine/GFFloatToLongFloor.h"

extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C

#pragma auto_inline(off)
// The same-TU body of 0x00A76F30 (the landed row's logic).
bool CNavQuadTree::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const
{
    for (std::vector<const std::list<C2DLineF>*>::const_iterator it = lineLists.begin(); it != lineLists.end(); ++it) {
        const std::list<C2DLineF>* lines = *it;
        for (std::list<C2DLineF>::const_iterator it2 = lines->begin(); it2 != lines->end(); ++it2) {
            const C2DLineF& line = *it2;
            if (line.GetHighestX() - area.TLX <= -DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetLowestX() - area.BRX >= DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetHighestY() - area.TLY <= -DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetLowestY() - area.BRY >= DELTA_NOTIONAL_ZERO)
                continue;
            if (area.ContainsPoint(line.Point1) || area.ContainsPoint(line.Point2) || line.IntersectsWith(area))
                return true;
        }
    }
    return false;
}
// STLport _vector.c growth paths (the header's __true_type body), out of line in retail.
template<> void std::vector<const std::list<C2DLineF>*>::_M_insert_overflow(iterator position, const value_type& x,
    const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size(); const size_type len = oldSize + (max)(oldSize, fillLength);
    value_type* newStart = this->_M_end_of_storage.allocate(len);
    value_type* newFinish = (value_type*)__copy_trivial(this->_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd) newFinish = (value_type*)__copy_trivial(position, this->_M_finish, newFinish);
    _M_clear(); _M_set(newStart, newFinish, newStart + len);
}
template<> void std::vector<unsigned __int64>::_M_insert_overflow(iterator position, const value_type& x,
    const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size(); const size_type len = oldSize + (max)(oldSize, fillLength);
    value_type* newStart = this->_M_end_of_storage.allocate(len);
    value_type* newFinish = (value_type*)__copy_trivial(this->_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd) newFinish = (value_type*)__copy_trivial(position, this->_M_finish, newFinish);
    _M_clear(); _M_set(newStart, newFinish, newStart + len);
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
        long minX = GFFloatToLongFloor((relPos.X - node->GetSize() * 0.5f) / 8.0f);
        long minY = GFFloatToLongFloor((relPos.Y - node->GetSize() * 0.5f) / 8.0f);
        long maxX = GFFloatToLongFloor((relPos.X + node->GetSize() * 0.5f) / 8.0f);
        long maxY = GFFloatToLongFloor((relPos.Y + node->GetSize() * 0.5f) / 8.0f);
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