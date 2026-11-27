// CCRC::Calc(const void*, unsigned long) -- table-driven CRC-32 of a byte buffer with a zero seed (table 0x0129A168), zero for an empty buffer
// Leaf: ?Calc@CCRC@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00950, 80 bytes, two stack arguments, `ret 8`, ecx not read.
// Name: Ego_r.pdb / Ego_d.pdb CCRC::Calc (the PDBs' builds have __fastcall `SIKPBXK` / seeded `SIKKPBXK` overloads; the retail
//   TLC body takes data and length on the stack, so it is modelled __stdcall). Callers: 0x00A00946, 0x00A5F6A6, 0x00A62586,
//   0x00A63896 -- small checksum methods passing `this + 4` and a fixed size.
// Shape: `crc = CRC32TAB[bytes[i] ^ (crc & 0xFF)] ^ (crc >> 8)` over an unsigned index; the explicit empty-buffer `return 0`
//   ahead of the loop leaves the loop's own `jbe` guard in place.
// NF479 (Codex pass 29 name fixes): the CRC-32 table at 0x0129A168 is the global ?CRC32TAB@@3QBKB (raw Ego_r / FableWin / Ego_d PDB publics).
extern const unsigned long CRC32TAB[256];          // 0x0129A168 (was the static member CCRC::Table)
class CCRC {
public:
    static unsigned long __stdcall Calc(const void* data, unsigned long length);
};

unsigned long __stdcall CCRC::Calc(const void* data, unsigned long length)
{
    unsigned long crc = 0;
    if (length == 0)
        return 0;
    const unsigned char* bytes = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i)
        crc = CRC32TAB[bytes[i] ^ (crc & 0xFF)] ^ (crc >> 8);
    return crc;
}