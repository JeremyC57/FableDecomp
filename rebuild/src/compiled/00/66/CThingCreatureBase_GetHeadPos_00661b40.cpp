// CThingCreatureBase::GetHeadPos(bool recalculate) const -- the creature's head position: the cached offset from its position when valid (and no recalculation is asked for), otherwise the graphic's EYE_SET_L dummy, the focus_eye_01 or TARGET_CURSOR_POINT helper, or 1.5 above the position; the offset from the position is cached and marked valid
// Leaf: ?GetHeadPos@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661B40, length 335 bytes (`ret 8`: hidden result + bool).
//   Signature from Ego_r.pdb `?GetHeadPos@CThingCreatureBase@@QBE?AVC3DVector@@_N@Z`.
// Callees: CThing::GetPos() const @ 0x004C73D0, CTCGraphicAppearance::GetDummyObject(const CCharString&, long,
//   unsigned long, C3DVector*, CRightHandedSet*, float) @ 0x004C08C0, CTCGraphicAppearance::GetHelperPoint(const
//   CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560; the names are the static CCharStrings
//   @ 0x013BAC54 "EYE_SET_L", 0x013BAC48 "focus_eye_01", 0x013BAC50 "TARGET_CURSOR_POINT"; 1.5f @ 0x0125AADC.
// Layout (retail): CThing::PTCGraphicAppearance +0x64 (Ego_r.pdb +0x74); CThingCreatureBase::HeadPosOffset +0x118
//   (PDB +0x13c: retail STLport vectors/lists are smaller) and its valid flag is bit 7 of the byte at +0x15c (mutable).
// Shape: the graphic is reached through an inline accessor (retail loads it into ecx before the pushes), the flag
//   default is `0` then `if (!recalculate) flags = 0x21`, and C3DVector has a declared memberwise copy ctor (the
//   returned `pos` is copied X through the FPU, Y/Z as dwords).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCharString { public: void* PStringData; };
extern CCharString g_CCharString_013bac54;              // "EYE_SET_L"
extern CCharString g_CCharString_013bac48;              // "focus_eye_01"
extern CCharString g_CCharString_013bac50;              // "TARGET_CURSOR_POINT"

struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* pos, float scale);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale);   // 0x004C08C0
};

class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    const C3DVector& GetPos() const;               // 0x004C73D0
    CTCGraphicAppearance* GetGraphicAppearance() const { return PTCGraphicAppearance; }
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad68[0x118 - 0x68];
    mutable C3DVector HeadPosOffset;                    // +0x118
    unsigned char _pad124[0x15c - 0x124];
    unsigned char _bits15c : 7;
    mutable unsigned char HeadPosOffsetValid : 1;       // +0x15c bit 7
    C3DVector GetHeadPos(bool recalculate) const;
};

C3DVector CThingCreatureBase::GetHeadPos(bool recalculate) const
{
    if (!recalculate && HeadPosOffsetValid)
        return GetPos() + HeadPosOffset;
    unsigned long flags = 0;
    if (!recalculate)
        flags = 0x21;
    C3DVector pos;
    CRightHandedSet set;
    if (!GetGraphicAppearance()->GetDummyObject(g_CCharString_013bac54, 0, flags, &pos, &set, 1.0f)
        && !GetGraphicAppearance()->GetHelperPoint(g_CCharString_013bac48, 0, flags, &pos, 1.0f)
        && !GetGraphicAppearance()->GetHelperPoint(g_CCharString_013bac50, 0, flags, &pos, 1.0f)) {
        pos = GetPos();
        pos.Z += 1.5f;
    }
    HeadPosOffset = pos - GetPos();
    HeadPosOffsetValid = true;
    return pos;
}