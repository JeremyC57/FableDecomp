// CTCBuyableHouse::GetRentMoneyPos(C3DVector& ret, long bag, long bags) -- where the bag-th of bags rent gold bags goes: one unit in front of the main sign post, then on a 0.7-radius circle around that point at bag/bags of a turn
// Leaf: ?GetRentMoneyPos@CTCBuyableHouse@@AAE_NAAVC3DVector@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C06D0, 312 bytes, `ret 0Ch`. Ego_r.pdb 0x006F8940 / FableWin.pdb 0x024B2D50 (private).
// Callees: CTCBuyableHouse::GetSignPostPos 0x006BFCD0, C2DVector::Rotate(float) 0x00A141D0. Globals: GFastOneOverSqrtTable
//   0x013CE558 (C3DVector::Normalise expanded), 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.7f 0x01255318.
// Ego_d locals: pos (C3DVector), rhs (CRightHandedSet); block { forward (C3DVector), offset (C2DVector) }.
// Matched (hand2c 2026-11-19): ported onto the shared headers with Ego_d's forward / offset locals; the PDB C3DVector::Normalise
//   expanded with its squared length summed Z, X, Y (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY: 0x006C06EC..0x006C070B) and the
//   components scaled X, Y, Z (the old local model's GetNormalised gave the Y operand-order residue at 0x006C0766).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS
#define FABLE_RETAIL_CTCBUYABLEHOUSE_GETRENTMONEYPOS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCBuyableHouse.h"

bool CTCBuyableHouse::GetRentMoneyPos(C3DVector& ret, long bag, long bags)
{
    C3DVector pos;
    CRightHandedSet rhs;
    if (GetSignPostPos(true, pos, &rhs)) {
        C3DVector forward = rhs.GetForward();
        forward.Normalise();
        pos += forward;
        C2DVector offset = C2DVector(rhs.GetForward()) * 0.7f;
        offset.Rotate((float)bag / bags);
        pos.X += offset.X;
        pos.Y += offset.Y;
        ret = pos;
        return true;
    }
    return false;
}
