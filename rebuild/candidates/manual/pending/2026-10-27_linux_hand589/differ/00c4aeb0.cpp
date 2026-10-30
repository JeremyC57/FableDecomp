// PKCBufferedReader::WaitForSlot(unsigned long slot) -- assert the slot index is below the slot count (+0x14; "Error: Request number (n) out of scope (count)", PKCBufferedReader.cpp line 84), then hand back the oldest outstanding buffer (ReturnBuffer) until the slot's owner byte (+4 map) reads free (0xFF); false when any hand-back reported no further result, else true
// Grade: DIFFER(284v281) -- frame only: retail keeps the trace CString, the CDebugBreak temporary and the `more` flag all in the dead `slot` argument slot ([esp+20h], no `sub esp,8`; prologue `mov eax,fs:[0]` first); VC7.1 3077/4035 opens an 8-byte frame for the string (+0x14) and the break byte (+0x13), so every [esp+N] differs by 8. Body instructions otherwise identical.
// Leaf: ?WaitForSlot@PKCBufferedReader@@QAE_NK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AEB0, 281 bytes (+ int3 padding), thiscall `ret 4`, EH-framed (the trace string,
//   state 0).
// Identity (descriptive; libpk has no PDB or RTTI): the trace names ".\PKCBufferedReader.cpp"; the reader's slot-owner map
//   +4 and slot count +0x14 of the landed ReturnBuffer 0x00C4AC70 model. One caller, 0x00C4B7EF in the reader's read
//   path: it computes the ring slot, calls this only while the slot is busy, and on false reports the slot through the
//   callback object at +0x6C. The catalog's "PKCBufferedReader_PrimeBuffer" fits nothing (it primes nothing), so the
//   method name is descriptive.
// Callees: PKCBufferedReader::ReturnBuffer(bool*) 0x00C4AC70 (landed); the libpk trace string of the sweep1 model
//   (PKCDefaultFileManager 0x00C1A230): CString::operator<< 0x00C0DEC0 (text) / 0x00C0DF00 (long) / 0x00C0DF10 (unsigned
//   long), ~CString 0x00C0BA40, CDebugBreak 0x00C16750 (DebugBreak), the empty string data _afxPchNil 0x0139C900 and the
//   assert hook g_pfnAssertHook 0x0143BB2C.
// Shape: the slot count is read once (edi) for the test and the message; the result flag lives in bl; `while` over the
//   owner byte rotates into a guarded do-while, and the `more` flag reuses the dead trace-string slot.
extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);     // 0x00C0DEC0
    CString& __fastcall operator<<(long value);           // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);  // 0x00C0DF10
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

class PKCBufferedReader {                       // vtable 0x012B8458, 0x70 bytes
public:
    virtual void v00();                         // slot 0: 0x00C4AB90
    unsigned char* m_slotOwners;                // +0x04 (request id per buffer slot, 0xFF = free)
    unsigned char  _pad08[0x14 - 0x08];
    unsigned long  m_slotCount;                 // +0x14
    void ReturnBuffer(bool* pMoreResults);      // 0x00C4AC70
    bool WaitForSlot(unsigned long slot);
};

bool PKCBufferedReader::WaitForSlot(unsigned long slot)
{
    bool ok = true;
    if (!(slot < m_slotCount)) {
        {
            CString line;
            line << ".\\PKCBufferedReader.cpp" << "(" << (long)84 << ") : " << "Error: Request number (" << slot
                 << ") out of scope (" << m_slotCount << ")" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    while (m_slotOwners[slot] != 0xFF) {
        bool more;
        ReturnBuffer(&more);
        if (!more)
            ok = false;
    }
    return ok;
}
