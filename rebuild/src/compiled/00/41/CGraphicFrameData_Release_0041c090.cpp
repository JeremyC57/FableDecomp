// CGraphicFrameData::Release() -- drop one reference; on the last one destroy the frame data (Uninitialise when initialised, release the counted texture) and free it
// Leaf: ?Release@CGraphicFrameData@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041C090, length 78 bytes (row padded to 80), plain `ret`.
// Name: Ego_r.pdb `?Release@CGraphicFrameData@@QAEXXZ` (with AddReference its reference-count pair); the catalog's
// "CGraphicFrameData_ReleaseReference" is descriptive. No direct callers in retail (an unreferenced instance).
// Body: `if (--ReferenceCount == 0) delete this;` -- `dec dword ptr [esi] ; jne` on the count at +0, then the inlined
// non-virtual destructor (Uninitialise 0x009FCBA0 when the IsInitialised bit 0x10 of the flag byte +0x1a is set; the
// PTexture counted pointer at +8: --count on its info block, re-read, at zero the block's destroy function (+4) on the
// object (+8, in ecx) and operator delete on the block; Data = Info = 0), then operator delete(this) @ 0x00BFE9BC.
// Layout: Ego_r.pdb CGraphicFrameData (as the landed ??_G 0x0041BDF0 and CGraphicFrame::operator= 0x0041C0E0 models):
// ReferenceCount +0, Data +4, PTexture +8, TLX..Pitch +0x10..+0x18, bit flags +0x1a; no engine/ header.
// Shape: /O2; the destructor chain must be `__forceinline` so `delete this` expands in place instead of calling the
// out-of-line scalar deleting destructor 0x0041BDF0.
// Model: local -- engine/CTexture.h exists, but the generated header is data-only (pack(1), no member functions, no virtuals:
// the vptr and counted pointers are flattened into raw words); the inlined release needs CTexture's virtual slots, so it is
// local.
void __cdecl operator delete(void* block);               // 0x00BFE9BC
struct CCPPointerInfo {
    long  RefCount;                                      // +0x00
    void  (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
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
    void Release();
    __forceinline ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
};
void CGraphicFrameData::Release()
{
    if (--ReferenceCount == 0)
        delete this;
}