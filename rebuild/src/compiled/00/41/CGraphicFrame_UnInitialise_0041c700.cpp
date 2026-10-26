// CGraphicFrame::UnInitialise() -- drop this frame's reference to its frame data, destroying the data at the last reference
// Leaf: ?UnInitialise@CGraphicFrame@@QAEXXZ
// Retail: Fable.exe @ 0x0041C700, 94 bytes, plain `ret`; Ego_r.pdb ?UnInitialise@CGraphicFrame@@QAEXXZ.
// Body: data = FrameData; if (data) { if (--data->ReferenceCount == 0) destroy and free data; FrameData = 0; } -- retail
// inlines ~CGraphicFrameData (Uninitialise 0x009FCBA0 when IsInitialised, then the PTexture counted-pointer release)
// and calls operator delete 0x00BFE9BC directly, as the ??_G 0x0041BDF0 body.
// Layout: Ego_r.pdb CGraphicFrame (4 bytes): FrameData +0; CGraphicFrameData as modelled at 0x0041BDF0.
// Shape: /O2 (16-aligned entry). `delete data` compiles to a call of the out-of-line ??_G (28 bytes; retail's ??_G
// 0x0041BDF0 has 12 other callers), so the destructor call and ::operator delete are spelled out; the pointer is bound to
// a local once (re-reading FrameData gives 101 bytes).
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CTexture;
class CGraphicFrameData {
public:
    unsigned long             ReferenceCount;            // +0x00
    void*                     Data;                      // +0x04
    CCountedPointer<CTexture> PTexture;                  // +0x08
    unsigned short            TLX;                       // +0x10
    unsigned short            TLY;                       // +0x12
    unsigned short            Width;                     // +0x14
    unsigned short            Height;                    // +0x16
    unsigned short            Pitch;                     // +0x18
    unsigned char             Alpha : 1;                 // +0x1a bit 0
    unsigned char             BooleanAlpha : 1;          // bit 1
    unsigned char             UsesSurface : 1;           // bit 2
    unsigned char             SectionOfTPage : 1;        // bit 3
    unsigned char             IsInitialised : 1;         // bit 4
    unsigned char             ScreenGraphic : 1;         // bit 5
    void Uninitialise();                                 // 0x009FCBA0
    ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;                        // +0x00
    void UnInitialise();
};
void CGraphicFrame::UnInitialise()
{
    CGraphicFrameData* data = FrameData;
    if (data)
    {
        if (--data->ReferenceCount == 0)
            { data->~CGraphicFrameData(); ::operator delete(data); }
        FrameData = 0;
    }
}