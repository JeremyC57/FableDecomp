// COverriddenChickenSign::SetWordA(unsigned short value) -- store the record's word A (+0x0a) through the embedded parameter record and mark it overridden (bit 0x10 of the set mask)
// Leaf: ?SetWordA@COverriddenChickenSign@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C204C0, 23 bytes (+9 int3), `__fastcall`, `ret 4`. Descriptive names: no PDB or
//   RTTI covers the libpk audio library; the manifest's "bind2nd<std::greater<long>,long>" is propagated.
// Identity: one of a run of nine identical-shape setters 0x00C203C0..0x00C204C0 (set-mask bits 1, 2, 4, 8, 0x10, 0x20,
//   0x80, 0x100, 0x200), each forwarding to one CChickenSign field setter of the record at +8 and OR-ing its bit into the
//   dword at +0. The only caller, the descriptor loader 0x00C27C10, calls this one when bit 0x10 of the descriptor's
//   byte +0x26 is set, passing the descriptor's word +0x44. CChickenSign is chunk 433/435's descriptive name for the
//   packed 0x14-byte sound-parameter record (word A at +0x0a).
// Callee: CChickenSign::SetWordA @ 0x00C1BBC0 (`mov ax,[esp+4] ; mov [ecx+0Ah],ax ; ret 4`).
// Shape: the forwarded call first (the argument re-pushed as a dword, the sub-object by `lea ecx,[esi+8]`), then the
//   read-modify-write `or dword ptr [esi],10h` with `this` kept in esi across the call.
struct CChickenSign {                           // the packed parameter record (chunk 433/435 model)
    unsigned char  m_level;                     // +0x00
    unsigned char  m_offset;                    // +0x01
    unsigned char  m_flags;                     // +0x02
    unsigned char  m_byteD;                     // +0x03
    short          m_wordB;                     // +0x04
    unsigned short m_wordC;                     // +0x06
    short          m_scale;                     // +0x08
    unsigned short m_wordA;                     // +0x0a
    unsigned short m_wordE;                     // +0x0c
    unsigned short m_time;                      // +0x0e
    unsigned short m_rangeMin;                  // +0x10
    unsigned short m_rangeMax;                  // +0x12
    void SetWordA(unsigned short value);        // 0x00C1BBC0
};

struct COverriddenChickenSign {                 // a parameter record plus the mask of fields set on it
    unsigned long  m_setMask;                   // +0x00
    unsigned long  m_reserved04;                // +0x04
    CChickenSign   m_sign;                      // +0x08
    void SetWordA(unsigned short value);
};

void COverriddenChickenSign::SetWordA(unsigned short value)
{
    m_sign.SetWordA(value);
    m_setMask |= 0x10;
}