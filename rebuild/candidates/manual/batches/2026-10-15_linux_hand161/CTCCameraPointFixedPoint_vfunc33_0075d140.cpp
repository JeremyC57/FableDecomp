// CTCCameraPointFixedPoint::vfunc_33(CPersistContext& context) -- persist TrackThing (default true) and the three LookVector components (default 0)
//
// Retail 0x0075D140, 128 bytes (`ret 4`). Vtable 0x0123AF8C (CTCCameraPointFixedPoint) slot 33.
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0, Transfer<float> @ 0x00410620 (each
// `(name, value&, const default&)`). Keys 0x01269DE4 "TrackThing", 0x01269DD4 "LookVector.X",
// 0x01269DC4 "LookVector.Y", 0x01269DB4 "LookVector.Z".
// Layout: Ego_r.pdb CTCCameraPointFixedPoint (packed, 0x50): TrackThing +0x40, LookVector +0x41.
// Shape: the defaults are literals bound to the const references; VC homes them in the frame word
// (the bool) and in the dead context-argument slot (each 0.0f re-stored before its call).
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0
};

#pragma pack(push, 1)
struct C3DVector { float X, Y, Z; };

struct CTCCameraPointFixedPoint {
    virtual void v00();
    unsigned char _pad04[0x40 - 0x04];      // +0x04 CTCBase + CTCCameraPointDefinitionBase
    bool          TrackThing;               // +0x40
    C3DVector     LookVector;               // +0x41
    unsigned char _pad4d[3];

    void vfunc_33(CPersistContext& context);
};
#pragma pack(pop)

void CTCCameraPointFixedPoint::vfunc_33(CPersistContext& context)
{
    context.Transfer("TrackThing", TrackThing, true);
    context.Transfer("LookVector.X", LookVector.X, 0.0f);
    context.Transfer("LookVector.Y", LookVector.Y, 0.0f);
    context.Transfer("LookVector.Z", LookVector.Z, 0.0f);
}
