// CVolumeTexture::GetVolumeLevel(unsigned int level) -- wrap one mip level of the Direct3D volume texture in a CVolume returned by value
// Leaf: GetVolumeLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67EA0, length 115 bytes, `ret 8` (hidden result + level).
// Label: the catalog's CPlayer::UninitModeQuestCompletionUI is propagated (the real one is the landed 0x00633B40);
// the local's vtable 0x0129C840 is CVolume (rtti-vtables.tsv), the interface call is IDirect3DVolumeTexture9 slot 18
// GetVolumeLevel(UINT, IDirect3DVolume9**), and the result is built with the CVolume copy constructor 0x00A673E0;
// the method name is descriptive.
// Callees: IDirect3DVolumeTexture9::GetVolumeLevel (COM slot 18, +0x48, `this` pushed), CBaseClass::CBaseClass @ 0x0099A2D0,
// CVolume::InitialiseFromVolume(IDirect3DVolume9*) @ 0x00A67770 (manifest: CSurface::InitialiseFromPreallocatedSurface),
// CVolume::CVolume(const CVolume&) @ 0x00A673E0, CVolume::ReleaseVolume(bool) @ 0x00A67460, CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: the out-pointer local is homed in the dead level-argument slot; `CVolume v(p); return v;` through an inline
// converting constructor (its by-value argument is read into esi before the base constructor)
// (no NRVO: copy into the hidden result, then the inline destructor stores the vtable and releases).
struct IDirect3DVolume9;

struct IDirect3DVolumeTexture9 {
    virtual long __stdcall v00(); virtual long __stdcall v01(); virtual long __stdcall v02(); virtual long __stdcall v03();
    virtual long __stdcall v04(); virtual long __stdcall v05(); virtual long __stdcall v06(); virtual long __stdcall v07();
    virtual long __stdcall v08(); virtual long __stdcall v09(); virtual long __stdcall v10(); virtual long __stdcall v11();
    virtual long __stdcall v12(); virtual long __stdcall v13(); virtual long __stdcall v14(); virtual long __stdcall v15();
    virtual long __stdcall v16();
    virtual long __stdcall v17();
    virtual long __stdcall GetVolumeLevel(unsigned int level, IDirect3DVolume9** volume);   // slot 18 (+0x48)
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CVolume : public CBaseClass { public:                                    // vtable 0x0129C840
    IDirect3DVolume9* m_pVolume;                            // +4
    CVolume() : m_pVolume(0) { }
    CVolume(IDirect3DVolume9* volume) : m_pVolume(0) { InitialiseFromVolume(volume); }
    CVolume(const CVolume& other);                          // 0x00A673E0
    ~CVolume() { ReleaseVolume(true); }
    void InitialiseFromVolume(IDirect3DVolume9* volume);    // 0x00A67770
    void ReleaseVolume(bool flag);                          // 0x00A67460
};

class CVolumeTexture { public:
    void*                    vfptr;                         // +0
    IDirect3DVolumeTexture9* m_pTexture;                    // +4
    CVolume GetVolumeLevel(unsigned int level);
};

CVolume CVolumeTexture::GetVolumeLevel(unsigned int level)
{
    IDirect3DVolume9* volume;
    m_pTexture->GetVolumeLevel(level, &volume);
    CVolume result(volume);
    return result;
}