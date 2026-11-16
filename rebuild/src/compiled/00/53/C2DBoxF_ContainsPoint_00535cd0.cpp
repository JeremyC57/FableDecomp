// C2DBoxF::ContainsPoint(const C2DVector& point) const -- half-open float box test
// Leaf: ?ContainsPoint@C2DBoxF@@QBE_NABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535CD0, length 66 bytes. No callees.
// Layout from the PDB (C2DBoxF: TLX +0, TLY +4, BRX +8, BRY +0xc; C2DVector: X +0, Y +4).
// Four ordered x87 compares, the point loaded first each time (`fld [p] ; fcomp [member]`):
// p.X >= TLX (`test ah,1 ; jne` fails on C0), p.X < BRX (`test ah,5 ; jp`), then the same for Y.
// Name fix (NF34, 2026-11-16): Ego_r.pdb + FableWin.pdb both spell ?ContainsPoint@C2DBoxF@@QBE_NABVC2DVector@@@Z (bool); the
// callers test al (0x00537DB9 / 0x00537DD9). `return a && b && c && d;` as bool emits retail's `mov eax,1` / `xor eax,eax`
// (the engine/C2DBoxF.h CONTAINSPOINT_INLINE note); the row now uses the shared C2DBoxF / C2DVector headers.
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"

bool C2DBoxF::ContainsPoint(const C2DVector& point) const
{
    return point.X >= TLX && point.X < BRX && point.Y >= TLY && point.Y < BRY;
}
