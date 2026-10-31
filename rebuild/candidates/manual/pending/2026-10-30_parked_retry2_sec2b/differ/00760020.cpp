// Grade: DIFFER(367v360) 30 difflines (was 41): IsSamePoint sums stepwise (d = dy*dy; d += dx*dx), giving retail's dy-first squaring; left: dead Z copy of points[0] into previous, iterator in ebx (retail esi) and current in esi/edi/ebp (retail edi/ebp/ebx) (retry wave 2 sec2b)
// Was: DIFFER(367v360) -- frame (0x40), edge/temp layout and branch structure match; retail copies only X,Y of points[0] into previous (no dead Z copy), keeps the iterator in esi and current in edi/ebp/ebx (ours ebx / esi,edi,ebp), and squares dy before dx (fld st0 ; fmul st1). Tried: C2DVector temporaries + operator- (395v360), C3DVector user copy ctor (382v360, previous goes to the x87 stack), edge-member checks (395v360), float-ctor line + plain C2DVector (this file).
// NShape::IsLineIntersectingWithOthersInList(const C2DLineF& line, const vector<C3DVector>& points) -- walk the closed polygon's edges and count those that are the line itself (either direction) or cross it; more than four means the line cuts the shape
// Leaf: ?IsLineIntersectingWithOthersInList@NShape@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00760020, 360 bytes, __fastcall (line ecx, points edx), plain `ret`.
// PDB: ?IsLineIntersectingWithOthersInList@NShape@@YI_NABVC2DLineF@@ABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Callees: C2DLineF::IntersectsWith(const C2DLineF&, C2DVector*, float) 0x00A56C80. Data: the 2D tolerance float
//   0x0129BA3C (read as a variable: pushed from memory), as in the landed C2DBoxI::ClipLineTo.
// Shape: previous point = points[0]; per edge take the next point (wrapping to points[0] once, which ends the walk),
//   build the edge line, and count it when its ends coincide with the line's ends (tolerance on the squared distance)
//   or it intersects the line; result `count > 4`.
extern const float g_C2DIntersectionTolerance;          // 0x0129BA3C

class C2DVector {
public:
    float X, Y;
};
class C3DVector {
public:
    float X, Y, Z;
};
class C2DLineF {
public:
    C2DVector Start;                                    // +0x00
    C2DVector End;                                      // +0x08
    C2DLineF(float x1, float y1, float x2, float y2) { Start.X = x1; Start.Y = y1; End.X = x2; End.Y = y2; }
    bool IntersectsWith(const C2DLineF& other, C2DVector* point, float tolerance);   // 0x00A56C80
};

namespace std {
template <class _Tp> class allocator {};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}

namespace NShape {
inline bool IsSamePoint(const C2DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float d = dy * dy;
    d += dx * dx;
    return d < g_C2DIntersectionTolerance;
}

bool __fastcall IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points)
{
    long count = 0;
    std::vector<C3DVector>::const_iterator it = points.begin();
    C3DVector previous = *it;
    ++it;
    bool finished = false;
    do {
        C3DVector current;
        if (it == points.end()) {
            current = *points.begin();
            finished = true;
        }
        else {
            current = *it;
            ++it;
        }
        C2DLineF edge(previous.X, previous.Y, current.X, current.Y);
        if ((IsSamePoint(edge.Start, line.Start) && IsSamePoint(edge.End, line.End))
            || (IsSamePoint(edge.Start, line.End) && IsSamePoint(edge.End, line.Start)))
            ++count;
        else {
            C2DVector intersection;
            if (edge.IntersectsWith(line, &intersection, g_C2DIntersectionTolerance))
                ++count;
        }
        previous = current;
    } while (!finished);
    return count > 4;
}
}
