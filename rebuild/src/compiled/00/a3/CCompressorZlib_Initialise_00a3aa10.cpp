// CCompressorZlib::Initialise() -- reset the zlib stream (no input/output buffers yet), install the compressor's own zalloc/zfree hooks and clear the initialised / compressing / error flags
// Leaf: ?Initialise@CCompressorZlib@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3AA10, 38 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?Initialise@CCompressorZlib@@AAEXXZ (private, void(void)). Its 12 callers are the CCompressorZlib
//   constructor sites: the out-of-line constructor 0x0052ADA0 calls CACompressor::CACompressor(GetRequiredWorkspaceSize())
//   (0x00A3AEA0 / 0x00A3A7D0), stores the vtable 0x01245974 (RTTI .?AVCCompressorZlib@@) and then this; the other 11
//   inline the same sequence. The stored function pointers are the PDB's private static __cdecl
//   CCompressorZlib::Alloc(void*, unsigned, unsigned) 0x00A3A9C0 (overflow-checked malloc of items * size) and
//   CCompressorZlib::Free(void*, void*) 0x00A3AA00 (free of the address). The earlier generated stub (struct C::Init,
//   module "Init"; manifest "CVO_OracleMarker::Init") was a propagated label.
// Layout: Ego_r.pdb CCompressorZlib = CACompressor (32 bytes there, 28 in retail: STLport's 12-byte vector) +
//   z_stream_s Stream (+0x20 -> retail +0x1c: next_in +0x1c, avail_in +0x20, next_out +0x28, avail_out +0x2c, zalloc +0x3c,
//   zfree +0x40) + bool ZlibInitialised / Compressing / ErrorFlag (+0x58.. -> retail +0x54..+0x56); local model
//   (engine/CCompressorZlib.h has the PDB size).
// Shape: the zero is materialised once in eax and reused for the byte flags.
struct internal_state;
typedef void* (__cdecl* alloc_func)(void* opaque, unsigned int items, unsigned int size);
typedef void  (__cdecl* free_func)(void* opaque, void* address);

struct z_stream_s {                             // zlib 1.x, 56 bytes
    unsigned char*  next_in;                    // +0x00
    unsigned int    avail_in;                   // +0x04
    unsigned long   total_in;                   // +0x08
    unsigned char*  next_out;                   // +0x0c
    unsigned int    avail_out;                  // +0x10
    unsigned long   total_out;                  // +0x14
    char*           msg;                        // +0x18
    internal_state* state;                      // +0x1c
    alloc_func      zalloc;                     // +0x20
    free_func       zfree;                      // +0x24
    void*           opaque;                     // +0x28
    int             data_type;                  // +0x2c
    unsigned long   adler;                      // +0x30
    unsigned long   reserved;                   // +0x34
};

class CACompressor {                            // retail 0x1c bytes
public:
    virtual ~CACompressor();
    void*         Workspace;                    // +0x04
    unsigned long WorkspaceSize;                // +0x08
    bool          CompressionOn;                // +0x0c
    unsigned char OwnWorkspace[0x0c];           // +0x10 vector<unsigned char> (STLport, 12 bytes)
};

class CCompressorZlib : public CACompressor {
public:
    z_stream_s Stream;                          // +0x1c
    bool       ZlibInitialised;                 // +0x54
    bool       Compressing;                     // +0x55
    bool       ErrorFlag;                       // +0x56
private:
    static void* __cdecl Alloc(void* opaque, unsigned int items, unsigned int size);   // 0x00A3A9C0
    static void  __cdecl Free(void* opaque, void* address);                            // 0x00A3AA00
    void Initialise();
};

void CCompressorZlib::Initialise()
{
    Stream.next_in = 0;
    Stream.next_out = 0;
    Stream.avail_in = 0;
    Stream.avail_out = 0;
    Stream.zalloc = Alloc;
    Stream.zfree = Free;
    ZlibInitialised = false;
    Compressing = false;
    ErrorFlag = false;
}
