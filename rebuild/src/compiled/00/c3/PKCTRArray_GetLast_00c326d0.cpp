// PKCTRArray::GetLast() -- the last entry of the pointer array: 0 when the array is empty; a null last entry traces "Error: Null item" (PKCTRArray.cpp line 212) and breaks; returns entry count-1
// Leaf: ?GetLast@PKCTRArray@@UAEPAXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C326D0, 224 bytes, thiscall `ret`, EH-framed (the trace string, state 0).
// Identity (descriptive method name; libpk has no PDB or RTTI): slot 12 of the PKCTRArray vtable 0x012B4E80 (its only
//   reference, 0x012B4EB0), after GetFirst 0x00C325F0 (slot 11, line 204); the trace names ".\PKCTRArray.cpp".
//   The catalog's gapscan name is a placeholder.
// Callees: the trace string's `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger
//   break temporary 0x00C16750; globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
// Shape: the empty return reuses the zero count already in eax (no xor); both reads index `[block + count*4 - 4]`.
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

// libpk PKCTRArray.cpp (no PDB): vtable 0x012B4E80, entry block +0x04, capacity +0x08, count +0x0C (as the landed
// ResizeAllocation 0x00C32950 and destructor 0x00C32BF0).
class PKCTRArray {
public:
    void**        m_pEntries;                 // +0x04
    unsigned long m_capacity;                 // +0x08
    unsigned long m_count;                    // +0x0C
    virtual void* GetFirst();                 // slot 11 (0x00C325F0)
    virtual void* GetLast();                  // slot 12 (0x00C326D0)
};

void* PKCTRArray::GetLast()
{
    if (m_count == 0)
        return 0;
    if (m_pEntries[m_count - 1] == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCTRArray.cpp" << "(" << 212 << ") : " << "Error: Null item" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pEntries[m_count - 1];
}