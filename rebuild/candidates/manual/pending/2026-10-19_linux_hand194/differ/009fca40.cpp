// Grade: DIFFER(ours v retail) -- 167v218: lane gap, the two TLX/TLY roundings are a bare-dword `fistp` through a pointer homed in the dead booleanAlpha/alpha slots (the inline-asm FloatToLong helper family, GOTCHAS 0xBE6790); C conversions call __ftol2 (or `/QIfist` a qword fistp). The prologue and the counted-pointer assignment match.
// CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float u, float v, unsigned long width, unsigned short height, bool alpha, bool booleanAlpha) -- make the frame a section of a texture page: take the texture, set size / pitch and alpha flags, the top-left corner from the normalised u/v (times 32767, rounded), surface + section + initialised, not a screen graphic; returns true
// Leaf: InitialiseAsSurfaceSection
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FCA40, 218 bytes, `ret 1Ch`.
// Callees: the old texture's count-block destroy (+4, object in ecx), operator delete @ 0x00BFE9BC. Float pool
//   0x0129C820 = 32767.0f.
// Layout: Ego_r.pdb CGraphicFrameData (see InitialiseAsSurface 0x009FC950): PTexture +8, TLX +0x10, TLY +0x12, Width
//   +0x14, Height +0x16, Pitch +0x18, flag byte +0x1a.
// Lane gap: the two roundings are the bare-dword `fistp` through a pointer (`lea edx,[esp+28h] ; mov [esp+24h],edx ;
//   fld [esp+10h] ; mov edx,[esp+24h] ; fistp dword ptr [edx]`) -- the inline-asm FloatToLong helper family (GOTCHAS
//   0xBE6790 / PackVertexNormal); plain C conversions go through __ftol2.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CTexture;

template <class T> struct CCountedPointer {
    struct SInfo {
        long m_references;
        void (__fastcall* m_pfnDestroy)(T*);
        T*   m_pObject;
    };
    T*     Data;
    SInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        SInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

struct CGraphicFrameData {
    unsigned long             ReferenceCount;      // +0x00
    void*                     Data;                // +0x04
    CCountedPointer<CTexture> PTexture;            // +0x08
    unsigned short            TLX;                 // +0x10
    unsigned short            TLY;                 // +0x12
    unsigned short            Width;               // +0x14
    unsigned short            Height;              // +0x16
    unsigned short            Pitch;               // +0x18
    unsigned char             Alpha : 1;           // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float u, float v, unsigned long width,
                                    unsigned short height, bool alpha, bool booleanAlpha);
};

bool CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float u, float v,
                                                   unsigned long width, unsigned short height, bool alpha,
                                                   bool booleanAlpha)
{
    UsesSurface = true;
    PTexture = texture;
    Width = (unsigned short)width;
    Pitch = (unsigned short)width;
    Height = height;
    Alpha = alpha;
    BooleanAlpha = booleanAlpha;
    float x = u * 32767.0f;
    TLX = (unsigned short)(long)x;
    float y = v * 32767.0f;
    TLY = (unsigned short)(long)y;
    ScreenGraphic = false;
    SectionOfTPage = true;
    IsInitialised = true;
    return true;
}
