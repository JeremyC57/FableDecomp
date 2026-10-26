struct CTextureResource;
struct CTextureResourceVTable { void* reserved0; void* reserved4; void (__stdcall *Release)(CTextureResource*); };
struct CTextureResource { CTextureResourceVTable* vtable; };
class CTexture { public: CTextureResource* resource; void Uninitialise(); };
void CTexture::Uninitialise() {
    if (resource != 0) resource->vtable->Release(resource);
}