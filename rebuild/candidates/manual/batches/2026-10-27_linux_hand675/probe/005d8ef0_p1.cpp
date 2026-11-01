// CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation orientation) -- private static: the right-handed set for one of the twelve inventory object orientations
// Leaf: ?GetRHSetFromOrientation@CTCInventoryItem@@CI
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
