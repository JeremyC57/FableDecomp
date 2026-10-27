// CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float u, float v, unsigned long width, unsigned short height, bool alpha, bool booleanAlpha) -- make the frame a section of a texture page: take the texture, set size / pitch and alpha flags, the top-left corner from the normalised u/v (times 32767, rounded), surface + section + initialised, not a screen graphic; returns true
// Leaf: InitialiseAsSurfaceSection
// Retail-asm: 0x009FCAD0 `mov edx, dword ptr [esp + 0x24]` ; 0x009FCAD4 `fistp dword ptr [edx]` and 0x009FCAF9 `mov edx, dword ptr [esp + 0x24]` ; 0x009FCAFD `fistp dword ptr [edx]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FCA40; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Both texel coordinates go through the pointer-out helper (the pointer homed in the dead booleanAlpha / alpha slots).
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

// the engine's pointer-out inline-assembly float-to-long helper (the landed 0x00B1B5E0 uses the same body): the result pointer is
// homed in a frame word and reloaded into edx for the bare dword fistp
__forceinline void FloatToLong(long* result, float value)
{
    __asm fld value
    __asm mov edx, result
    __asm fistp dword ptr [edx]
}
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
    long rounded;
    float x = u * 32767.0f;
    FloatToLong(&rounded, x);
    TLX = (unsigned short)rounded;
    float y = v * 32767.0f;
    FloatToLong(&rounded, y);
    TLY = (unsigned short)rounded;
    ScreenGraphic = false;
    SectionOfTPage = true;
    IsInitialised = true;
    return true;
}
