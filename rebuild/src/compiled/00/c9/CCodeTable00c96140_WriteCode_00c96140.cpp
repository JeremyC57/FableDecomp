// CCodeTable_00c96140::WriteCode(long index, CBitstream_00c96140* stream) -- emit symbol `index` of a libpk code table: the code word m_codes[index] is written with the length m_pLengths->m_lengths[index] bits, and that length is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C96140, length 43 bytes (the oracle row carries 5 bytes of int3 padding).
// Callees: CBitstream::WriteBits @ 0x00C90FC0 (`__fastcall`: ecx = stream, edx = value, stack = bit count; the same callee as
// CBitstream::FlushByte @ 0x00C911B0). Shape: the libpk module is compiled `__fastcall` (/Gr): `this` in ecx (edi), the index in edx (esi),
// the stream on the stack (`[esp+0Ch]`, `ret 4`); the length table is reached through the pointer at +0xc and its array at +8, the code
// table is the array at +0x14; the length is re-read after the call for the return value. The manifest label
// "CEnvironment::GetLocalEnvironmentThemeIndexFromGlobal" is propagated; the class is named by address.
struct CBitstream_00c96140;
void __fastcall WriteBits_00c90fc0(CBitstream_00c96140* stream, long value, long count);   // 0x00C90FC0

struct CCodeLengths_00c96140 {
    unsigned char _pad[8];
    long*         m_lengths;                       // +0x08
};

struct CCodeTable_00c96140 {
    unsigned char          _pad0[0xc];
    CCodeLengths_00c96140* m_pLengths;             // +0x0c
    unsigned char          _pad1[4];
    long*                  m_codes;                // +0x14
    long __fastcall WriteCode(long index, CBitstream_00c96140* stream);
};

long __fastcall CCodeTable_00c96140::WriteCode(long index, CBitstream_00c96140* stream)
{
    WriteBits_00c90fc0(stream, m_codes[index], m_pLengths->m_lengths[index]);
    return m_pLengths->m_lengths[index];
}