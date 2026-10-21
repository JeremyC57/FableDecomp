// CGraphicInfo::InitialiseFramesAsSurfaceSections(const CCountedPointer<CTexture>& texture, long a, long b, long c, long d) -- cut the source surface into the graphic's frames left to right, wrapping to the next row, building each frame from its section (UV origin = position / surface size) and handing it to the frame slot
// Grade: DIFFER(351v343) -- first-pass model: retail precomputes the float surface sizes once, keeps the row end in ebx and x/y/index in frame words (0x24-byte frame), the frame temporary and its release follow
// Leaf: InitialiseFramesAsSurfaceSections
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FE780, length 341 bytes (`ret 14h`).
// Callees: CGraphicFrame::InitialiseAsSurface @ 0x009FCB60 (the section's UV origin as a by-value C2DVector), CGraphicInfoFrame::
//   InitialiseFrame @ 0x009FE550, CGraphicFrameData::Uninitialise @ 0x009FCBA0, operator delete @ 0x00BFE9BC.
// Layout: the landed CGraphicInfo::UnloadFrame 0x009FE8E0 header model (Width +0, Height +2, FrameWidth +6, FrameHeight +8,
//   FrameCount +0xA, Frames +0x28 (16-byte slots)); the frame's counted data block {RefCount, ..., flags +0x1A, counted
//   texture +0x8/+0xC}.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct CPointerInfo { long RefCount; void (__fastcall* Destroy)(void*); void* Data; };
struct CTexture;
struct CTexturePointer { CTexture* Data; CPointerInfo* Info; };
struct CGraphicFrameData {
    long            RefCount;                           // +0x00
    unsigned char   _pad04[4];
    void*           TextureData;                        // +0x08
    CPointerInfo*   TextureInfo;                        // +0x0C
    unsigned char   _pad10[0xa];
    unsigned char   Flags;                              // +0x1A
    void Uninitialise();                                // 0x009FCBA0
    ~CGraphicFrameData()
    {
        if (Flags & 0x10)
            Uninitialise();
        if (TextureInfo) {
            --TextureInfo->RefCount;
            if (TextureInfo->RefCount == 0) {
                TextureInfo->Destroy(TextureInfo->Data);
                delete TextureInfo;
            }
        }
        TextureData = 0;
        TextureInfo = 0;
    }
};
struct CGraphicFrame {
    CGraphicFrameData* Data;
    CGraphicFrame() : Data(0) {}
    ~CGraphicFrame()
    {
        if (Data) {
            --Data->RefCount;
            if (Data->RefCount == 0)
                delete Data;
        }
    }
    bool InitialiseAsSurface(const CTexturePointer& texture, C2DVector origin, long width, long height, long a, long b);   // 0x009FCB60
};
struct CGraphicInfoFrame {
    unsigned char _body[0x10];
    void InitialiseFrame(CGraphicFrame* frame, long c, long d);   // 0x009FE550
};
#pragma pack(push, 1)
struct CGraphicHeader {
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
};
#pragma pack(pop)
struct CGraphicInfo {
    CGraphicHeader     Header;                          // +0x00
    unsigned char      _pad0c[0x1c];
    CGraphicInfoFrame* Frames;                          // +0x28
    void InitialiseFramesAsSurfaceSections(const CTexturePointer& texture, long a, long b, long c, long d);
};

void CGraphicInfo::InitialiseFramesAsSurfaceSections(const CTexturePointer& texture, long a, long b, long c, long d)
{
    long frameHeight = Header.FrameHeight;
    long width = Header.Width;
    long height = Header.Height;
    long frameWidth = Header.FrameWidth;
    long x = 0;
    long y = 0;
    for (unsigned long i = 0; i < Header.FrameCount; ++i) {
        long right = x + frameWidth;
        if (right > width) {
            x -= width;
            y += frameHeight;
        }
        CGraphicFrame frame;
        frame.InitialiseAsSurface(texture, C2DVector((float)x / (float)width, (float)y / (float)height), frameWidth, frameHeight, a, b);
        Frames[i].InitialiseFrame(&frame, c, d);
        x += frameWidth;
    }
}
