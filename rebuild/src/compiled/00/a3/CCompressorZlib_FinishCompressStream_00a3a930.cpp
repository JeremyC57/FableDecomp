// CCompressorZlib::FinishCompressStream  @ 0x00a3a930
// Leaf: ?FinishCompressStream@CCompressorZlib@@QAEXXZ
// Naming wave 29 (family zlib): the stream is closed by _z_deflateEnd 0x00C00AE0 (both PDBs; was the invented _EndDeflateStream); own Leaf
//   ?FinishCompressStream@CCompressorZlib@@QAEXXZ (Ego_r.pdb / FableWin.pdb).
// Clears an active zlib deflate stream if one is open.

struct z_stream_s;
extern "C" int __cdecl z_deflateEnd(z_stream_s* strm);   // 0x00C00AE0 (zlib built with Z_PREFIX: both PDBs spell _z_deflateEnd)

class CCompressorZlib { public:
    char pad[0x1c];      // 0x00
    char zstream[0x38];  // 0x1c: z_stream-like block
    char active;         // 0x54: "stream open" flag
    void FinishCompressStream();
};

void CCompressorZlib::FinishCompressStream()
{
    if (this->active) {
        z_deflateEnd((z_stream_s*)&this->zstream);
        this->active = 0;
    }
}