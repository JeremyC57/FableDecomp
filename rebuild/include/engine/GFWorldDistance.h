#ifndef FABLE_ENGINE_GFWORLDDISTANCE_H
#define FABLE_ENGINE_GFWORLDDISTANCE_H
#include "engine/C3DVector.h"
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
// Original debug01f284e0, locals sqr_dist/sqr_comp.
inline bool FABLE_FASTCALL GFWorld2DDistanceSmallerThan(const C3DVector& pos1,const C3DVector& pos2,float compare,bool using_delta_notional_zero) {
 float sqr_dist=GFSqr(pos1.X-pos2.X)+GFSqr(pos1.Y-pos2.Y);
 float sqr_comp=using_delta_notional_zero?compare:GFSqr(compare);
 return sqr_dist<sqr_comp;
}
#ifdef FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER
// Both original PDB names and FW01d76780: local squared distance and comparison.
inline bool FABLE_FASTCALL GFWorld3DDistanceSmallerThan(const C3DVector& pos1,const C3DVector& pos2,float compare){
 float sqr_dist=GFSqr(pos1.X-pos2.X)+GFSqr(pos1.Y-pos2.Y)+GFSqr(pos1.Z-pos2.Z);
 float sqr_comp=GFSqr(compare);
 return sqr_dist<sqr_comp;
}
#endif
#ifdef FABLE_RETAIL_GFGETWORLD2DDISTANCE
// Retail opt-in (chunk Q17, row 0x009711D0): FableWin.pdb ?GFGetWorld2DDistance@@YIMABVC3DVector@@0@Z (0x025B03E0), inline in
// retail (0x0097131E: Y difference, then X difference, both squared, fsqrt). Same local-style body as GFWorld2DDistanceSmallerThan.
#include <math.h>
inline float FABLE_FASTCALL GFGetWorld2DDistance(const C3DVector& pos1,const C3DVector& pos2) {
 float dy=pos1.Y-pos2.Y;
 float dx=pos1.X-pos2.X;
 float sqr_dist=dx*dx+dy*dy;
 return (float)sqrt(sqr_dist);
}
#endif
#endif
