// CTCLook::IsWithinYZHeadRange(float angleYZ) -- true (1) when the angle lies inside the look def's vertical head range [-(down / 360), up / 360]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751FC0, length 58 bytes. Callees: none.
// Constant: 1/360 (0.0027777778f) @ 0x01238E00 (the def stores the limits in degrees, the angles are in turns).
// Layout (local model, no engine header): PDef (CDefPointer<CLookDef const>, the def pointer as its first word) at +0x14;
// the def's YZ limits at +0x34 (up) and +0x38 (down), names descriptive.
// Shape: an `&&` of two float compares written with the angle on the left -- `test ah,1 ; jne` is `!(angle <= up)`,
// `test ah,41h ; jp` is `!(angle >= -down)` (unordered fails both); the result is a full-register 0/1 (`long`, not bool);
// `def->x * K` loads the member first and multiplies by the pool constant.
class CLookDef { public:
    unsigned char _pad_0x00[0x34];
    float         MaxHeadAngleYZUp;              // +0x34 (degrees)
    float         MaxHeadAngleYZDown;            // +0x38 (degrees)
};

struct CLookDefPointer {
    const CLookDef* m_pDef;                      // +0x00
    const CLookDef* operator->() const { return m_pDef; }
};

class CTCLook { public:
    unsigned char   _pad_0x00[0x14];
    CLookDefPointer PDef;                        // +0x14
    long IsWithinYZHeadRange(float angleYZ);
};

long CTCLook::IsWithinYZHeadRange(float angleYZ)
{
    if (angleYZ <= PDef->MaxHeadAngleYZUp * 0.0027777778f && angleYZ >= -(PDef->MaxHeadAngleYZDown * 0.0027777778f))
        return 1;
    return 0;
}