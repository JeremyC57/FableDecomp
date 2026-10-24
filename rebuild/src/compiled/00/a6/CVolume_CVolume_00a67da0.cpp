// CVolume::CVolume(IDirect3DVolume9* volume) -- construct an empty volume wrapper and initialise it from a volume-texture mip level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67DA0, length 39 bytes (48 with int3 padding).
// Leaf: ??0CVolume
// Callees: CBase::CBase() @ 0x0099A2D0, CVolume::InitialiseFromVolumeTextureMipmap(IDirect3DVolume9*) @ 0x00A67770
// (thiscall, `ret 4`: releases a held volume through slot 2, stores and AddRefs the new one, then caches its
// description). Vtable 0x0129C840 = CVolume (rtti-vtables.tsv).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer" is propagated, and so
// is the callee label "CSurface::InitialiseFromPreallocatedSurface". FableWin.pdb has
// ??0CVolume@@QAE@PAUIDirect3DVolume9@@@Z and ?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z;
// the model is the landed CVolume copy constructor's (0x00A673E0: CBase base, the D3D volume at +4).
// Shape: base call; the argument push, the vtable store and the m_pVolume(0) initialiser scheduled before the
// body's Initialise call.
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct IDirect3DVolume9 : IUnknownLike { };
namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;
struct CVolume : CBase {
    IDirect3DVolume9* m_pVolume;            // +0x04
    CVolume(IDirect3DVolume9* volume);
    void InitialiseFromVolumeTextureMipmap(IDirect3DVolume9* volume);   // 0x00A67770
};
CVolume::CVolume(IDirect3DVolume9* volume) : m_pVolume(0)
{
    InitialiseFromVolumeTextureMipmap(volume);
}