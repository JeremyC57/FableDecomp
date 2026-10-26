// CVolumeTexture::ForwardToVolumeLevel(unsigned long, void*) -- fetch the wrapped D3D volume of mip level `level` as a temporary CVolume, hand `argument` to its (empty in this build) helper @ 0x00A67450, then release the temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69550, length 63 bytes.
// Leaf: ForwardToVolumeLevel
// Callees: CVolumeTexture::GetVolumeLevel @ 0x00A67EA0 (__fastcall on `this`, CVolume returned through the
// hidden pointer; it calls IDirect3DVolumeTexture9 slot 18 GetVolumeLevel on the wrapped texture at +4),
// the one-argument CVolume helper @ 0x00A67450 (a bare `ret 4`; also called from CVolumeTexture code at
// 0x00A687AC / 0x00A6898F with a local record's address), CVolume::ReleaseVolume @ 0x00A67460
// (releases +4 through IUnknown::Release and nulls it; its argument is unused), CBase::~CBase @ 0x0099A2E0.
// Identity: the manifest label "CPaintMapDialog::InitNumberBox" is propagated and rejected; the body has no
// callers or vtable references. `this` goes untouched into GetVolumeLevel, which is a CVolumeTexture
// method (all its eleven callers are CVolumeTexture code), so the function is modelled as a CVolumeTexture
// member; its real name is not recoverable from the bytes. Twin of 0x009FCD40 (helper 0x00A67440).
// Shape: `GetVolumeLevel(level).Helper(argument)` -- VC pushes the member call's argument before it
// evaluates the object expression, so `argument` is pushed first and stays on the stack across the
// GetVolumeLevel call (`ret 8`) to be consumed by the helper; the temporary's inline CVolume destructor
// stores the CVolume vtable (0x0129C840), calls ReleaseVolume(true) and the out-of-line CBase destructor.
struct IDirect3DVolume9;

struct CBase {
    virtual ~CBase();                               // 0x0099A2E0
};

class CVolume : public CBase { public:                            // vtable 0x0129C840
    IDirect3DVolume9* m_pVolume;                    // +0x04
    void ReleaseVolume(bool release);               // 0x00A67460
    void Helper_00a67450(void* argument);                    // 0x00A67450 (empty)
    virtual ~CVolume() { ReleaseVolume(true); }
};

class CVolumeTexture { public:
    void ForwardToVolumeLevel(unsigned long level, void* argument);
public:
    CVolume GetVolumeLevel(long) const;   // 0x00A67EA0
};

void CVolumeTexture::ForwardToVolumeLevel(unsigned long level, void* argument)
{
    GetVolumeLevel(level).Helper_00a67450(argument);
}