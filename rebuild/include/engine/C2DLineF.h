#ifndef FABLE_ENGINE_C2DLINEF_H
#define FABLE_ENGINE_C2DLINEF_H
// C2DLineF (new header, chunk 736): Ego_r.pdb layout Point1 +0, Point2 +8 (C2DVector), sizeof 0x10 (struct_layouts_egor.tsv).
// The extent getters are the landed bodies 0x00A56B80..0x00A56C10 ((float)Min<double> / Max<double> over const-reference
// templates on double temporaries), inline in CNavQuadTree::IsAreaBlockedByLines 0x00A76F30; IntersectsWith(const C2DBoxF&)
// is out of line at 0x00A57BD0. Max / Min are the Lionhead lib templates the landed 0x00A76F30 row spells.
#include "engine/C2DVector.h"
class C2DBoxF;
template <class T> inline const T& Max(const T& left, const T& right) { return left < right ? right : left; }
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
class C2DLineF {
public:
    C2DVector Point1;                                                    // +0x00
    C2DVector Point2;                                                    // +0x08
#ifdef FABLE_RETAIL_C2DLINEF_POINT_ON_LINE_PASS24
    C2DVector GetPointOnInfiniteLine(const C2DVector& point) const;
    float GetPointOnLine(const C2DVector& point,C2DVector& point_on_line,float radius,
                         C2DVector* point_on_infinite_line,bool* off_start) const;
#endif
#ifdef FABLE_RETAIL_NAV_WIDTH_PASS30
    float GetDist(const C2DBoxF&, float*, float*, bool*) const;
    bool IsWithinDist(const C2DVector&, float);
#endif
#ifdef FABLE_RETAIL_NAV_BOX_DISTANCE_PASS30
    // FableWin 0324D72C, 0324D740, 0324D797 and 0324D7AF, in the box distance overload.
    C2DLineF() {}
    bool ContainedBy(const C2DBoxF&) const;
    void Initialise(float x1,float y1,float x2,float y2) {
        Point1.X=x1; Point1.Y=y1; Point2.X=x2; Point2.Y=y2;
    }
    float GetDist(const C2DLineF&,float*,float*,bool*) const;
#endif
    float GetLowestX() const { return (float)Min<double>(Point1.X, Point2.X); }
    float GetHighestX() const { return (float)Max<double>(Point1.X, Point2.X); }
    float GetLowestY() const { return (float)Min<double>(Point1.Y, Point2.Y); }
    float GetHighestY() const { return (float)Max<double>(Point1.Y, Point2.Y); }
    bool IntersectsWith(const C2DBoxF& box) const;                       // 0x00A57BD0
#ifdef FABLE_RETAIL_C2DLINEF_FROM3D
    // Retail opt-in (p25J/p25L, row 0x00760020 NShape::IsLineIntersectingWithOthersInList): FableWin.pdb ??0C2DLineF@@QAE@ABVC3DVector@@0@Z, inline in retail (the
    // two end points' X / Y copied into the argument temporaries). Needs FABLE_RETAIL_C2DVECTOR_FROM3D. No layout change.
    C2DLineF(const C3DVector& point1, const C3DVector& point2) : Point1(point1), Point2(point2) { }
#endif
#ifdef FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE
    // Retail opt-in (p25L, row 0x00760020 NShape::IsLineIntersectingWithOthersInList): FableWin.pdb and Ego_r.pdb
    // ?IntersectsWith@C2DLineF@@QBE_NABV1@AAVC2DVector@@M@Z, out of line in retail at 0x00A56C80 (landed). Declaration only.
    bool IntersectsWith(const C2DLineF& line, C2DVector& intersection, float tolerance) const;   // 0x00A56C80
#endif
#ifdef FABLE_RETAIL_C2DLINEF_SQUAREDDIST
    // Retail opt-in (chunk 875, row 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity): Ego_r.pdb / FableWin.pdb
    // ??0C2DLineF@@QAE@ABVC2DVector@@0@Z (inline in retail: the two end points copied member-wise into the frame, 0x00A1F330..
    // 0x00A1F358) and ?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z (non-const, out of line at 0x00A57770: `push ecx ;
    // lea ecx,[esp+88h] ; call 0x00A57770` at 0x00A1F397). Declarations only for the out-of-line member: no layout change.
    C2DLineF(const C2DVector& point1, const C2DVector& point2) : Point1(point1), Point2(point2) { }
    float GetSquaredDist(const C2DVector& point);                        // 0x00A57770
#endif
};
#endif // FABLE_ENGINE_C2DLINEF_H
