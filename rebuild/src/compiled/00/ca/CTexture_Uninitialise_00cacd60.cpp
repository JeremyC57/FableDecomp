// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/ctlutil.cpp) ??1CBaseDispatch@@QAE@XZ -- 13 bytes verified against Fable.exe 0x00CACD60
struct CTextureResource;
struct CTextureResourceVTable { void* reserved0; void* reserved4; void (__stdcall *Release)(CTextureResource*); };
struct CTextureResource { CTextureResourceVTable* vtable; };
class CTexture { public: CTextureResource* resource; void Uninitialise(); };
void CTexture::Uninitialise() {
    if (resource != 0) resource->vtable->Release(resource);
}