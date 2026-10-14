// CDebugString::AddString() -- bump the string counter under the debug-string lock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C330, length 40 bytes.
// Callees: CProfileLock::CProfileLock(CProfileSection*) @ 0x00C0FFC0, CProfileLock::~CProfileLock @ 0x00C0FFE0.
// Shape: an 8-byte scoped lock (section pointer + entry tick, see wave 23f) built in the frame over the
// section at +0x10, the counter at +8 incremented through eax between the constructor and destructor calls.
// PDB CDebugString (28 bytes): +8 vector<CWideString> Strings, +0x18 long NoStrings; retail reads +8 as the
// counter and locks at +0x10, so the layout is modelled locally (the retail vector is 12 bytes).
struct CProfileSection { unsigned char _opaque[4]; };   // only its address is taken

struct CProfileLock {                  // 8-byte scoped lock
    CProfileSection* m_pSection;
    long             m_entryTick;
    CProfileLock(CProfileSection* section);   // 0x00C0FFC0
    ~CProfileLock();                          // 0x00C0FFE0
};

struct CDebugString {
    unsigned char   _pad[8];           // +0x00
    long            m_count;           // +0x08
    unsigned char   _pad2[4];          // +0x0c
    CProfileSection m_section;         // +0x10 (opaque; only its address is taken)
    void AddString();
};

void CDebugString::AddString()
{
    CProfileLock lock(&m_section);
    ++m_count;
}
