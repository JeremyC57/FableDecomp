#ifndef FABLE_ENGINE_GFWORLDDISTANCE_H
#define FABLE_ENGINE_GFWORLDDISTANCE_H
#include "engine/C3DVector.h"
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
#ifdef FABLE_RETAIL_KID_EVADE_PASS23
#include "engine/C2DVector.h"
// Original PUBLIC01b4f090 and PDB locals pos1,pos2,dx,dy; donor64bytes.
inline float FABLE_FASTCALL GFGet2DDistanceSquared(const C2DVector& pos1,const C2DVector& pos2) {
 float dx=pos2.X-pos1.X;
 float dy=pos2.Y-pos1.Y;
 return dx*dx+dy*dy;
}
#endif
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
#if defined(FABLE_RETAIL_GFGETWORLD2DDISTANCE_X_FIRST) && !defined(FABLE_RETAIL_GFGETWORLD2DDISTANCE)
// Retail opt-in (egod e10, row 0x007993F0 CTCShopKeeper::WitnessedCrime): the same FableWin.pdb inline
// ?GFGetWorld2DDistance@@YIMABVC3DVector@@0@Z (0x025B03E0) with the X difference first, as retail 0x007994A3 does it (`fld [eax]` /
// `fsub [esi]`, then the +4 pair). FITTED: FableWin's /Od size of this text (28 instructions / 73 bytes, fwsize) equals 0x025B03E0
// exactly; the Y-first text above sizes 27/70. The Y-first default stays for landed 0x009711D0 (retail 0x0097131E does Y first).
// Use one of the two opt-ins per TU, never both.
#include <math.h>
inline float FABLE_FASTCALL GFGetWorld2DDistance(const C3DVector& pos1,const C3DVector& pos2) {
 float dx=pos1.X-pos2.X;
 float dy=pos1.Y-pos2.Y;
 float sqr_dist=dx*dx+dy*dy;
 return (float)sqrt(sqr_dist);
}
#endif
#if defined(FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER_DECL) && !defined(FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER)
// Opt-in (B24C7, row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim): the same Ego_r / FableWin
// ?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z declared only, for TUs that call the out-of-line copy (retail 0x006614F0,
// called from 0x0070D99B) instead of expanding the inline body above.
bool FABLE_FASTCALL GFWorld3DDistanceSmallerThan(const C3DVector& pos1,const C3DVector& pos2,float compare);
#endif
#endif
