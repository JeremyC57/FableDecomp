// CTCCarryable::GetCarriedPositionInfo(C3DVector&, C3DVector* basis, long helper, long index) -- when carried, fetch the carrier's helper frame (position, up, forward), build the carried item's offset from the right/up/forward axes scaled by the item's offsets and scale, transform it and the item's own basis through the carrier's orientation matrix and the carrier frame, and return the final world position and the three basis vectors
// Grade: DIFFER(694v1326) -- first approximate draft of the carried-frame math (helper frame, cross product, scaled offset, two 3x3 multiplies, basis and position out); retail is fully inlined x87 matrix code (row-by-column products into a 0x30 matrix copied with rep movsd) whose operand order and spill slots this draft does not reproduce
// Leaf: GetCarriedPositionInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x007055D0, 1328 bytes, `ret 0x10`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
inline C3DVector Cross(const C3DVector& a, const C3DVector& b) { return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
struct CMatrix33 { C3DVector Right, Up, Forward; };
struct CMatrix34 { C3DVector Right, Up, Forward, Position; };
struct SHelperFrame { C3DVector Up; C3DVector Forward; };
inline C3DVector operator*(const C3DVector& v, const CMatrix33& m)
{
    return C3DVector(v.X * m.Right.X + v.Y * m.Up.X + v.Z * m.Forward.X,
                     v.X * m.Right.Y + v.Y * m.Up.Y + v.Z * m.Forward.Y,
                     v.X * m.Right.Z + v.Y * m.Up.Z + v.Z * m.Forward.Z);
}
CMatrix33* __fastcall Multiply(CMatrix33* out, const CMatrix33& a, const CMatrix33& b);   // 0x00A35B70
struct CThing;
bool GetHelperFrame(CThing* thing, CMatrix34* frame, SHelperFrame* helper, long helperIndex, long index);   // 0x006A1A00
struct CThing {
    unsigned char _pad[0x60];
    struct CController {
        void GetOrientation(CMatrix33* out, long index);   // vtable +0x11c (stand-in)
    }* Controller;                                      // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

class CTCCarryable {
public:
    void* __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CThing* Carrier;                                    // +0x10
    float RightOffset;                                  // +0x14
    float Scale;                                        // +0x18
    float UpOffset;                                     // +0x1c
    float ForwardOffset;                                // +0x20
    void GetCarrierFrame(CMatrix33* out, long helperIndex, long index, const CMatrix34& frame);   // 0x00705240
    void ApplyOffset(const C3DVector& position, C3DVector* out, const CMatrix33& basis, const CMatrix34& frame, long helperIndex);   // 0x00705190
    bool GetCarriedPositionInfo(C3DVector& position, C3DVector* basis, long helperIndex, long index);
};

bool CTCCarryable::GetCarriedPositionInfo(C3DVector& position, C3DVector* basis, long helperIndex, long index)
{
    if (!Carrier)
        return false;
    CMatrix34 frame;
    SHelperFrame helper;
    if (!GetHelperFrame(ParentThing, &frame, &helper, helperIndex, index))
        return false;
    float scale = Scale;
    C3DVector right = Cross(helper.Up, helper.Forward);
    C3DVector offset = (right * RightOffset + helper.Up * UpOffset + helper.Forward * ForwardOffset) * scale;
    CMatrix33 carrier;
    GetCarrierFrame(&carrier, helperIndex, index, frame);
    CMatrix33 orientation;
    ParentThing->Controller->GetOrientation(&orientation, index);
    CMatrix33 combined;
    Multiply(&combined, orientation, *(CMatrix33*)&frame);
    CMatrix33 full;
    Multiply(&full, carrier, combined);
    C3DVector up = helper.Up * full;
    C3DVector forward = helper.Forward * full;
    C3DVector side = offset * full;
    C3DVector result;
    ApplyOffset(ParentThing->GetPosition(), &result, full, frame, helperIndex);
    basis[0] = side;
    basis[1] = up;
    position = C3DVector(frame.Position.X - result.X, frame.Position.Y - result.Y, frame.Position.Z - result.Z) + forward;
    return true;
}
