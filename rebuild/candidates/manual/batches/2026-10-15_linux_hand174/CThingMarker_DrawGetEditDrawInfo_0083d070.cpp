// CThingMarker::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- the physical-thing editor draw info, relabelled "Marker - <script name>" and coloured magenta-ish (R 255, G 128, B 255, A 255)
// Leaf: ?DrawGetEditDrawInfo@CThingMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D070, length 136 bytes (`ret 8`, + int3 padding).
// Callees: CThingPhysical::DrawGetEditDrawInfo @ 0x006A4E90, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// operator+(const CCharString&, const char*) @ 0x0099F600 ("Marker - " @ 0x012730C4; __fastcall, hidden result in ecx),
// operator+(const CCharString&, const CCharString&) @ 0x0099F570 (__fastcall), CCharString::operator= @ 0x0099EFB0,
// CCharString::~CCharString @ 0x0099EAE0 (three temporaries).
// Layout: PDB CEditEngineDrawInfo (40 bytes): Pos +0, Radius +0xc, Fullname +0x10, Colour (CRGBColour B,G,R,A) +0x14, PlayerOwner +0x18;
// retail CThing ScriptName at +0x74 (PDB +0x84; see the landed CThingGameObject::ConstructFromParams 0x008388D0).
// Shape: one full expression `Fullname = CCharString("") + "Marker - " + ScriptName;` (temporaries in the dead argument slots,
// destroyed in reverse); the colour is the inline (r, g, b, a) setter as in the landed CThing::DrawGetEditDrawInfo 0x004C74B0.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CRGBColour {
    unsigned char B;                            // +0
    unsigned char G;                            // +1
    unsigned char R;                            // +2
    unsigned char A;                            // +3
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
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
};
struct CThingPhysical {
    void*         __vftable;
    unsigned char _pad04[0x74 - 0x04];
    CCharString   ScriptName;                   // +0x74 (retail)
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // 0x006A4E90
};
struct CThingMarker : CThingPhysical {
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;
};

bool CThingMarker::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const
{
    if (CThingPhysical::DrawGetEditDrawInfo(time, info)) {
        info->Fullname = CCharString("") + "Marker - " + ScriptName;
        info->Colour.Set(0xff, 0x80, 0xff, 0xff);
        return true;
    }
    return false;
}
