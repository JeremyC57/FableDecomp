// CThingTrackNode::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- the physical-thing editor draw info, coloured blue (R 128, G 128, B 255, A 255), named "TrackNode" and without an effect radius
// Leaf: ?DrawGetEditDrawInfo@CThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008374F0, length 128 bytes (`ret 8`). Vtable CThingTrackNode 0x01272CE4
// slot 25 (manifest "vfunc_25").
// Callees: CThingPhysical::DrawGetEditDrawInfo @ 0x006A4E90, CCharString::CCharString(const char*, long) @ 0x0099EBF0
// ("" @ 0x0122D70E), operator+(const CCharString&, const char*) @ 0x0099F600 ("TrackNode" @ 0x01245BE8; __fastcall,
// hidden result in ecx, left in edx), CCharString::operator= @ 0x0099EFB0, CCharString::~CCharString @ 0x0099EAE0.
// Layout: PDB CEditEngineDrawInfo (40 bytes): Pos +0, Radius +0xc, Fullname +0x10, Colour (CRGBColour B,G,R,A) +0x14,
// PlayerOwner +0x18, EffectRadius +0x1c, UsesEffectRadius +0x20, AngleXY +0x24.
// Shape: the colour is a CRGBColour temporary whose (r, g, b, a) constructor assigns R, G, B, A in its body
// (+2/+1/+0/+3 byte stores into a dead argument slot) copied as one dword; then the name expression
// `CCharString("") + "TrackNode"` (temporaries in the dead argument slots, destroyed in reverse), then the flag.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
struct CRGBColour {
    unsigned char B;                            // +0
    unsigned char G;                            // +1
    unsigned char R;                            // +2
    unsigned char A;                            // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};
struct C3DVector { float X, Y, Z; };
struct CEditEngineDrawInfo {
    C3DVector   Pos;                            // +0x00
    float       Radius;                         // +0x0c
    CCharString Fullname;                       // +0x10
    CRGBColour  Colour;                         // +0x14
    long        PlayerOwner;                    // +0x18
    float       EffectRadius;                   // +0x1c
    bool        UsesEffectRadius;               // +0x20
    float       AngleXY;                        // +0x24
};
struct CThingPhysical {
    void* __vftable;
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // 0x006A4E90
};
struct CThingTrackNode : CThingPhysical {
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;
};

bool CThingTrackNode::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const
{
    if (CThingPhysical::DrawGetEditDrawInfo(time, info)) {
        info->Colour = CRGBColour(0x80, 0x80, 0xff, 0xff);
        info->Fullname = CCharString("") + "TrackNode";
        info->UsesEffectRadius = false;
        return true;
    }
    return false;
}