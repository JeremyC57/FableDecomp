// CGraphicFrameData::`scalar deleting destructor'(unsigned int flags) -- uninitialise if initialised, release the texture pointer, optionally free the frame
// Leaf: ??_GCGraphicFrameData@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0041BDF0, 85 bytes, `ret 4`; 12 direct callers in 0x00B729A0..0x00B7E1D5 (no vptr: non-virtual
// destructor, `delete frame`). Ego_r.pdb CGraphicFrameData (no user destructor symbol in the PDB name list; Uninitialise
// and Release are).
// Body: ~CGraphicFrameData: if (IsInitialised) Uninitialise() (bit 4 of the flag byte +0x1a; 0x009FCBA0, Ego_r.pdb
// ?Uninitialise@CGraphicFrameData@@QAEXXZ); then the PTexture member's destructor (+0x08: --count on the info block,
// re-read; at zero its destroy thunk on the item and operator delete 0x00BFE9BC on the block; Data = Info = 0); then
// operator delete(this) when bit 0 of the flags is set.
// Layout: Ego_r.pdb CGraphicFrameData -- ReferenceCount +0, Data +4, PTexture +8 (CCountedPointer<CTexture>), TLX..Pitch
// +0x10..+0x18, bit flags at +0x1a (Alpha, BooleanAlpha, UsesSurface, SectionOfTPage, IsInitialised = 0x10,
// ScreenGraphic); no engine/ header. CCountedPointer model as the landed 0x00448860 row.
// Shape: /O2 (16-aligned entry); the ??_G is emitted by a `delete frame` helper (not a retail function).
struct CCPPointerInfo {
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
void DeleteGraphicFrameData(CGraphicFrameData* frame)    // emission helper, not a retail function
{
    delete frame;
}