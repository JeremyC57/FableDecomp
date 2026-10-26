// CVolumeTexture::CVolumeTexture() -- construct an empty volume texture (no D3D texture, no data block)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A695C0, length 26 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (root of the engine's polymorphic classes, vtable 0x01231710).
// Label correction: the manifest calls this "CTargetPoint::CTargetPoint" (propagated); the vtable it stores,
// 0x0129DC18, is CVolumeTexture's (rtti-vtables.tsv).
// Layout (Ego_r.pdb CVolumeTexture, 16 bytes): vptr +0, PD3DVolumeTexture +4, ByteLength +8, PData +0xc.
// Shape: base call, the texture pointer and the data pointer cleared from one zero register, ByteLength left
// untouched, then the derived vtable store (plain initialisers precede it).
struct IDirect3DVolumeTexture9;

class CBaseClass { public:                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

class CVolumeTexture : public CBaseClass { public:                        // vtable 0x0129DC18
    IDirect3DVolumeTexture9* PD3DVolumeTexture;             // +0x04
    long                     ByteLength;                    // +0x08
    void*                    PData;                         // +0x0c
    CVolumeTexture();
    virtual void v00();
};

CVolumeTexture::CVolumeTexture()
    : PD3DVolumeTexture(0), PData(0)
{
}