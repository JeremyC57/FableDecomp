// CTexture::GetNextValidDimensions(const C2DExtentsI& requested, const CPixelFormat& format) -- the texture size the device will accept: the requested size unchanged, or each side rounded up to the next power of two when the device caps demand power-of-two textures
// Grade: DIFFER(336v351) -- two folds retail does not make: the caps test (ours `test byte ptr [eax+98h],2` for `(caps >> 1) & 1`, a bitfield, `& 2` or an inlined IsBitSet(value, 1); retail `mov ecx,[eax+98h] ; shr ecx,1 ; test cl,1`), and the zero side (ours drops the store of 0 into a side already known 0; retail computes each power into eax, `xor eax,eax` on the zero path, then `mov edi/ebx,eax`). Returning C2DExtentsI(next(w), next(h)) or assigning a temporary reorders the sides
// Leaf: ?GetNextValidDimensions@CTexture@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FA620, 349 bytes, static __fastcall (ecx = hidden C2DExtentsI result,
//   edx = requested, the unused format on the stack, `ret 4`). Callers: 0x00494A0D, 0x0049A153, CManagedTextureData::
//   CreateTexture 0x00A697C4 and CreateVolumeTexture 0x00A699F6 (both landed, declaring the same convention).
// Identity: Ego_r.pdb `?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z`.
// Layout (retail): GFGetSystemManager()->PDisplayManager.Data at +0x60; CDisplayManager D3DDeviceCaps (D3DCAPS9) at +0x5C,
//   so TextureCaps is +0x98 (bit 1 = D3DPTEXTURECAPS_POW2).
// Callees: GFGetSystemManager @ 0x009A4EC0 (__fastcall); HighestSetBit (out of line @ 0x0099FDB0) is expanded twice.
// Shape: the requested size is copied first (edi / ebx); the caps bit is read shifted (`shr ecx,1 ; test cl,1`; a bitfield or `& 2` gives `test byte ptr`); each
//   side becomes 0 for 0, else 1 << HighestSetBit(2 * side - 1).
struct C2DExtentsI {
    long Width;
    long Height;
    C2DExtentsI() {}
    C2DExtentsI(long w, long h) : Width(w), Height(h) {}
};

struct CPixelFormat;

class CDisplayManager {
public:
    unsigned char _pad_000[0x98];
    unsigned long TextureCaps;                      // +0x98 (D3DDeviceCaps +0x5C, TextureCaps +0x3C)
};

template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};

class CSystemManager {
public:
    unsigned char _pad_00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();                    // 0x009A4EC0

__forceinline long HighestSetBit(unsigned long value)      // out of line @ 0x0099FDB0
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}

__forceinline long GetNextPowerOfTwo(long value)
{
    if (value == 0)
        return 0;
    return 1 << HighestSetBit(value * 2 - 1);
}

class CTexture {
public:
    static C2DExtentsI __fastcall GetNextValidDimensions(const C2DExtentsI& requested, const CPixelFormat& format);
};

C2DExtentsI __fastcall CTexture::GetNextValidDimensions(const C2DExtentsI& requested, const CPixelFormat& format)
{
    C2DExtentsI size = requested;
    if ((GFGetSystemManager()->PDisplayManager.Data->TextureCaps >> 1) & 1) {   // D3DPTEXTURECAPS_POW2
        size.Width = GetNextPowerOfTwo(size.Width);
        size.Height = GetNextPowerOfTwo(size.Height);
    }
    return size;
}
