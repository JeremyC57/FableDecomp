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
    float GetLowestX() const { return (float)Min<double>(Point1.X, Point2.X); }
    float GetHighestX() const { return (float)Max<double>(Point1.X, Point2.X); }
    float GetLowestY() const { return (float)Min<double>(Point1.Y, Point2.Y); }
    float GetHighestY() const { return (float)Max<double>(Point1.Y, Point2.Y); }
    bool IntersectsWith(const C2DBoxF& box) const;                       // 0x00A57BD0
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
