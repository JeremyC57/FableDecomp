// CreateSharedOps() -- create the libpk reverb's shared DSP kernel table (CDspObject::s_pOps, 0x0143CCDC): the SIMD build (constructor 0x00C88A30, vtable 0x012BF1F4) when the CPU-feature byte 0x0143BC7C is set, else the x87 build (vtable 0x012BF02C, inline constructor), both through the table's class operator new (0x00C763C0); on success the reference count (0x0143CCE0) starts at 1
// Leaf: ?CreateSharedOps@@YA_NXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74360, 138 bytes, __cdecl, returns bool.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC74360 CreateHeroStatsListener builds the shared DSP ops table"):
//   - its only caller is agent444's landed CDspObject::CDspObject 0x00C74420 (`if (!s_pOps) CreateSharedOps(); else
//     ++s_refCount;`), which binds ?CreateSharedOps@@YA_NXZ to this address; CDspObject's destructor 0x00C74450 drops
//     s_refCount and frees s_pOps through 0x00C763D0 when it reaches 0;
//   - 0x012BF02C is agent444's x87 CDspOps table (slot 0 GetKind 0x00C73FB0, then the block kernels ScaleRampCopy
//     0x00C74130 .. Mix 0x00C74230); 0x012BF1F4 has the same nine-slot shape with the SIMD kernels 0x00C88A40..
//     0x00C88D80 and the same slot 8 (0x00C76880), so it derives from CDspOps;
//   - 0x0143BC7C is one of the four CPU-feature bytes written together at 0x00C52E80 (0x0143BC7B..0x0143BC7E); the SSE2
//     sound mixers 0x00C6B251.. test the same byte.
//   The CreateHeroStatsListener / listener names are a manifest description; descriptive names (libpk: no PDB/RTTI).
// Shape: the shared `push 4` before the branch; only the out-of-line SIMD constructor needs an EH state (its unwind
//   frees the block through the class operator delete 0x00C763D0).
extern bool g_bUseSimd;                                 // 0x0143BC7C (CPU-feature byte)

class CDspOps {                                         // vtable 0x012BF02C (x87 kernels)
public:
    CDspOps() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C763C0 (jumps to the library allocator 0x00C3C360)
    static void __cdecl operator delete(void* p);           // 0x00C763D0
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    virtual void v08();                                                                             // 0x00C76880 (shared)
};

class CDspOpsSimd : public CDspOps {                    // vtable 0x012BF1F4 (SIMD kernels)
public:
    CDspOpsSimd();                                      // 0x00C88A30
};

class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
    static CDspOps* s_pOps;                             // 0x0143CCDC
    static long     s_refCount;                         // 0x0143CCE0
};

bool CreateSharedOps()
{
    CDspOps* ops;
    if (g_bUseSimd)
        ops = new CDspOpsSimd;
    else
        ops = new CDspOps;
    CDspObject::s_pOps = ops;
    if (!ops)
        return false;
    CDspObject::s_refCount = 1;
    return true;
}
