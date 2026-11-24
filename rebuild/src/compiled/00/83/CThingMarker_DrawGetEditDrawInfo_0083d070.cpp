// CThingMarker::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- the physical-thing editor draw info, relabelled "Marker - <script name>" and coloured magenta-ish (R 255, G 128, B 255, A 255)
// Leaf: ?DrawGetEditDrawInfo@CThingMarker@@UBE_NMPAVCEditEngineDrawInfo@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D070, length 136 bytes (`ret 8`, + int3 padding).
// Callees: CThingPhysical::DrawGetEditDrawInfo @ 0x006A4E90, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// operator+(const CCharString&, const char*) @ 0x0099F600 ("Marker - " @ 0x012730C4; __fastcall, hidden result in ecx),
// operator+(const CCharString&, const CCharString&) @ 0x0099F570 (__fastcall), CCharString::operator= @ 0x0099EFB0,
// CCharString::~CCharString @ 0x0099EAE0 (three temporaries).
// Layout: PDB CEditEngineDrawInfo (40 bytes): Pos +0, Radius +0xc, Fullname +0x10, Colour (CRGBColour B,G,R,A) +0x14, PlayerOwner +0x18;
// retail CThing ScriptName at +0x74 (PDB +0x84; see the landed CThingGameObject::ConstructFromParams 0x008388D0).
// Shape: one full expression `Fullname = CCharString("") + "Marker - " + ScriptName;` (temporaries in the dead argument slots,
// destroyed in reverse); the colour is the inline (r, g, b, a) setter as in the landed CThing::DrawGetEditDrawInfo 0x004C74B0.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class CRGBColour { public:
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
#include "engine/C3DVector.h"
class CEditEngineDrawInfo { public:
    C3DVector   Pos;                            // +0x00
    float       Radius;                         // +0x0c
    CCharString Fullname;                       // +0x10
    CRGBColour  Colour;                         // +0x14
    long        PlayerOwner;                    // +0x18
};
class CThingPhysical { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    unsigned char _pad04[0x74 - 0x04];
    CCharString   ScriptName;                   // +0x74 (retail)
    virtual bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // slot 25, 0x006A4E90
};
class CThingMarker : public CThingPhysical { public:
    virtual bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // slot 25
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