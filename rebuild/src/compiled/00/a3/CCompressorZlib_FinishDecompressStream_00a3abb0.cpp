// CCompressorZlib::FinishDecompressStream()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3ABB0, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; push edi ; mov edi,ecx ; mov al,[edi+54h] ; xor ebx,ebx ; cmp al,bl ; je R ; push esi ; lea esi,[edi+1Ch] ; push esi ; call z_inflateEnd ; add esp,4 ; mov [esi],ebx ; mov [esi+0Ch],ebx ; mov [esi+4],ebx ; mov [esi+10h],ebx ; mov [edi+54h],bl ; pop esi ; R: pop edi ; pop ebx ; ret
// engine/CCompressorZlib.h (PDB): z_stream Stream at +0x20, ZlibInitialised at +0x58; retail has
// both 4 bytes earlier (one container precedes the stream). Ends an inflate session: inflateEnd
// (zlib, __cdecl, 0x00C038D0) on the embedded z_stream, then next_in / next_out / avail_in /
// avail_out are cleared (in that order) and the initialised flag dropped.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.
//
// Lane feedback: the zero stores go through the z_stream pointer (esi-relative), not through `this`;
// member-relative stores swap the esi/edi roles. RELOCATION_MATCH.
struct z_stream_s_Local {
    unsigned char* next_in;      // +0x00
    unsigned long  avail_in;     // +0x04
    unsigned long  total_in;     // +0x08
    unsigned char* next_out;     // +0x0C
    unsigned long  avail_out;    // +0x10
    unsigned char  _rest[0x24];  // 0x38-byte z_stream
};

extern "C" int __cdecl inflateEnd(z_stream_s_Local* stream);   // z_inflateEnd @ 0x00C038D0

// Retail CCompressorZlib view (PDB offsets +4; see the README container rule).
struct CCompressorZlib_Retail {
    unsigned char    _pad_0x00[0x1C];
    z_stream_s_Local Stream;             // +0x1C (PDB +0x20)
    bool             ZlibInitialised;    // +0x54 (PDB +0x58)
    void FinishDecompressStream();
};

void CCompressorZlib_Retail::FinishDecompressStream()
{
    // Tear the inflate state down and clear the stream's buffer pointers/counts through the
    // same z_stream pointer handed to inflateEnd (retail stores relative to that pointer).
    if (this->ZlibInitialised) {
        z_stream_s_Local* stream = &this->Stream;
        inflateEnd(stream);
        stream->next_in   = 0;
        stream->next_out  = 0;
        stream->avail_in  = 0;
        stream->avail_out = 0;
        this->ZlibInitialised = false;
    }
}