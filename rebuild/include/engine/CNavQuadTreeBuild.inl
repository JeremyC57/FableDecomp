#ifdef FABLE_RETAIL_NAVQUADTREE_BUILD_PASS30
#include "engine/GFFloatToLongFloor.h"
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
inline bool CNavQuadTree::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const
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
template<> inline void std::vector<const std::list<C2DLineF>*>::_M_insert_overflow(iterator position, const value_type& x,
    const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size(); const size_type len = oldSize + (max)(oldSize, fillLength);
    value_type* newStart = this->_M_end_of_storage.allocate(len);
    value_type* newFinish = (value_type*)__copy_trivial(this->_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd) newFinish = (value_type*)__copy_trivial(position, this->_M_finish, newFinish);
    _M_clear(); _M_set(newStart, newFinish, newStart + len);
}
template<> inline void std::vector<unsigned __int64>::_M_insert_overflow(iterator position, const value_type& x,
    const __true_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size(); const size_type len = oldSize + (max)(oldSize, fillLength);
    value_type* newStart = this->_M_end_of_storage.allocate(len);
    value_type* newFinish = (value_type*)__copy_trivial(this->_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd) newFinish = (value_type*)__copy_trivial(position, this->_M_finish, newFinish);
    _M_clear(); _M_set(newStart, newFinish, newStart + len);
}
inline bool CNavQuadTree::IsAreaClear(const CANavQuadTreeNode* node, bool& switchable, std::vector<unsigned __int64>& ids) const
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
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#endif
