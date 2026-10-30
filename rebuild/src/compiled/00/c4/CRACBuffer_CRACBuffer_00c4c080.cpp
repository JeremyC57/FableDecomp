// CRACBuffer::CRACBuffer(unsigned char* base, unsigned long size, unsigned long offset, unsigned long alignment) -- libpk random-access buffer view: store base / size / offset / alignment, warn "Blocksize <n> not appropriate" (PKRACBuffer.cpp line 11) unless the alignment is 1 or a non-zero multiple of 4, then derive the head padding (offset modulo the alignment) and the tail padding that rounds head + size up to a whole number of alignment units
// Leaf: ??0CRACBuffer@@QAE@PAEKKK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4C080, 294 bytes, thiscall `ret 10h`, EH-framed (state 0: the warning string).
// Identity: the libpk PKRACBuffer.cpp TU (its own trace names `.\PKRACBuffer.cpp`, line 11; Ghidra's source map puts
//   0x00C4C080 and 0x00C4C1D0 in pkracbuffer.cpp); the stores are the members the landed CRACBuffer rows read (base +0,
//   size +4, offset +8, alignment +0xC: GetAlignedSize 0x00C4BFF0, GetPointer 0x00C4C000, IsRangeAligned 0x00C4C050,
//   CheckAlignmentOffset 0x00C4C1D0). Its one caller is the bank loader 0x00C3BCB0 (call at 0x00C3BF86), which builds
//   the view on its stack and then probes it with the landed CRACBuffer getters. Class name as landed (descriptive:
//   libpk has no PDB or RTTI); the method is the constructor (it returns `this`). The manifest label "CRABuffer_ctor"
//   is a guess.
// Callees: the libpk trace-string block of the sweep1 model as landed in chunk 589 (0x00C25B10): CString::operator<<
//   0x00C0DEC0 (text) / 0x00C0DF00 (long) / 0x00C0DF10 (unsigned long), ~CString 0x00C0BA40, CDebugBreak 0x00C16750,
//   _afxPchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C.
// Shape: the string and the CDebugBreak temporary are homed by VC7.1 in the dead offset / base argument slots; the
//   padding arithmetic is written as retail computes it (`x - x / a * a`, `(x + a - 1) / a * a`: two `div` + `imul`,
//   no remainder register), the head padding stored after the second division.
extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);   // 0x00C0DF10
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct CRACBuffer {                             // 0x18 bytes
    unsigned char* m_pBase;                     // +0x00
    unsigned long  m_size;                      // +0x04
    unsigned long  m_offset;                    // +0x08
    unsigned long  m_alignment;                 // +0x0c
    unsigned long  m_headPadding;               // +0x10 offset modulo the alignment
    unsigned long  m_tailPadding;               // +0x14 bytes after head + size up to the next alignment boundary
    CRACBuffer(unsigned char* base, unsigned long size, unsigned long offset, unsigned long alignment);
};

CRACBuffer::CRACBuffer(unsigned char* base, unsigned long size, unsigned long offset, unsigned long alignment)
    : m_pBase(base), m_size(size), m_offset(offset), m_alignment(alignment)
{
    if (alignment != 1 && !(alignment > 0 && (alignment & 3) == 0)) {
        {
            CString line;
            line << ".\\PKRACBuffer.cpp" << "(" << (long)11 << ") : " << "Blocksize " << alignment << " not appropriate" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    m_headPadding = m_offset - m_offset / m_alignment * m_alignment;
    m_tailPadding = (m_size + m_headPadding + m_alignment - 1) / m_alignment * m_alignment - m_size - m_headPadding;
}