// CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation orientation) -- private static: the right-handed set for one of the twelve inventory object orientations
// Leaf: ?GetRHSetFromOrientation@CTCInventoryItem@@CI
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8EF0, 316 bytes (incl. the 12-entry jump table), static __fastcall: hidden
//   CRightHandedSet result in ecx, orientation in edx, plain `ret`.
// Identity: Ego_r.pdb ?GetRHSetFromOrientation@CTCInventoryItem@@CI?AVCRightHandedSet@@W4EInventoryObjectOrientation@@@Z
//   (private static fastcall). Callers: CTCInventoryItem::GetStartingRightHandedSet 0x005D9730 (landed, declares this
//   exact spelling at this address) and its instance twin 0x005D92F0. Catalog "_global::GetRHSetFromOrientation" lacks the class.
// Globals: the twelve orientation sets are the FableWin.pdb anonymous-namespace constants of this TU, in their
//   `dynamic initializer for` order (FableWin 0x03FE0080..0x03FE0690: DEFAULT, FORTY_FIVE_DEGREES, RIGHT_/LEFT_FORTY_FIVE,
//   UPSIDE_DOWN, UPSIDE_DOWN_FORTY_FIVE, UPSIDE_DOWN_RIGHT_/LEFT_, BACKWARDS, BACKWARDS_FORTY_FIVE, BACKWARDS_RIGHT_/LEFT_),
//   which is the jump-table case order. Cross-check against retail .data: case 0 0x0137CF74 = (0,-1,0)/(0,0,-1) (default),
//   case 4 0x0137CFBC = (0,1,0)/(0,0,-1) (upside down), case 8 0x0137D004 = (0,-1,0)/(0,0,1) (backwards) are fully
//   static; the 45-degree sets have a runtime-computed Up (zero in the image) and Ego_r lists dynamic initializers for
//   exactly the nine FORTY_FIVE_DEGREES names. Landed spellings g_DefaultRHSet / g_InventoryOrientationRHSet<N> are
//   invented (NAME_FIXES). The enumerator names of EInventoryObjectOrientation are not in either PDB dump, so the cases
//   are written as numbers. Their initializers live elsewhere in the retail TU and are not reconstructed here (extern).
// Callee: CRightHandedSet's implicit copy constructor ??0CRightHandedSet@@QAE@ABV0@@Z 0x0049CF80 (C3DVector's inline
//   memberwise copy makes it non-trivial). VC7.1 calls it out of line for the twelve cases and expands it for the
//   fall-through default, as retail does (first probe, the landed GetStartingRightHandedSet model).
enum EInventoryObjectOrientation { };
class C3DVector {                                       // 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class CRightHandedSet {                                 // 24 bytes
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // Ego_r ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z
};
namespace {
extern const CRightHandedSet DEFAULT_ORIENTATION;                              // 0x0137CF74
extern const CRightHandedSet FORTY_FIVE_DEGREES_ORIENTATION;                   // 0x013B8BCC
extern const CRightHandedSet RIGHT_FORTY_FIVE_DEGREES_ORIENTATION;             // 0x0137CF8C
extern const CRightHandedSet LEFT_FORTY_FIVE_DEGREES_ORIENTATION;              // 0x0137CFA4
extern const CRightHandedSet UPSIDE_DOWN_ORIENTATION;                          // 0x0137CFBC
extern const CRightHandedSet UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION;       // 0x013B8BE4
extern const CRightHandedSet UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION; // 0x0137CFD4
extern const CRightHandedSet UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION;  // 0x0137CFEC
extern const CRightHandedSet BACKWARDS_ORIENTATION;                            // 0x0137D004
extern const CRightHandedSet BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION;         // 0x013B8BFC
extern const CRightHandedSet BACKWARDS_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION;   // 0x0137D01C
extern const CRightHandedSet BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION;    // 0x0137D034
}
class CTCInventoryItem {
private:
    static CRightHandedSet __fastcall GetRHSetFromOrientation(EInventoryObjectOrientation orientation);
};
CRightHandedSet __fastcall CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation orientation)
{
    switch (orientation) {
    case 0:  return DEFAULT_ORIENTATION;
    case 1:  return FORTY_FIVE_DEGREES_ORIENTATION;
    case 2:  return RIGHT_FORTY_FIVE_DEGREES_ORIENTATION;
    case 3:  return LEFT_FORTY_FIVE_DEGREES_ORIENTATION;
    case 4:  return UPSIDE_DOWN_ORIENTATION;
    case 5:  return UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION;
    case 6:  return UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION;
    case 7:  return UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION;
    case 8:  return BACKWARDS_ORIENTATION;
    case 9:  return BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION;
    case 10: return BACKWARDS_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION;
    case 11: return BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION;
    }
    return DEFAULT_ORIENTATION;
}