// CEventSubscription_00c23e00::CEventSubscription(count) -- EH-framed constructor (vtable 0x012B3A9C; the manifest's "NScript_CEvent_InitializeActionParameters"; the landed 0x00C23C10 is the plain-`malloc` twin): the count at +4, +8 / +0xc cleared; a zero count leaves the two tables at +0x10 / +0x14 empty; otherwise a 44-byte-per-entry table is built with `new[]` over the class allocator (`malloc`, the raw block freed on unwind; `__ehvec_ctor` with 0x00C27AF0 / 0x00C271D0) and a word-per-entry table `malloc`ed, each failure raising "EMEM"-style warnings ("Memory allocation failure", d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h lines 63 and 65: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 -- the first string and trace in the dead argument slot, the second trace on a free frame byte; states 0 and 1), and every entry is initialised (@ 0x00C22920)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C23E00, oracle length 480 bytes. Built under /EHsc.
// Retail bytes:  ... (as 0x00C23C10, the entry table from `imul eax,eax,2Ch ; add eax,4 ; push eax ; call malloc ; ... mov [eax],edi ; call __ehvec_ctor` with 0x00C27AF0 / 0x00C271D0, the block freed on unwind; the entries initialised @ 0x00C22A40 every 2Ch bytes) ...
// The class is named by address.
void* __cdecl malloc(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c23e00)(const char* text);
extern PFNLOGSINK_00c23e00 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c23e00 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c23e00() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c23e00();                       // 0x00C0BA40
    CString_00c23e00* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c23e00* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c23e00 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c23e00();                         // 0x00C16750
};

void __cdecl free(void* block);
struct SEntry_00c23e00 {                    // 44 bytes per entry, built and torn down out of line
    unsigned char _body[0x2c];
    SEntry_00c23e00();                           // 0x00C27AF0
    ~SEntry_00c23e00();                          // 0x00C271D0
    static void* operator new[](unsigned int size) { return malloc(size); }
    static void operator delete[](void* block) { free(block); }
};

struct CEventSubscription_00c23e00 {
    virtual void v00();
    unsigned long m_count;                 // +4
    long m_8;
    long m_c;
    SEntry_00c23e00* m_pEntries;           // +0x10
    long* m_pSlots;                        // +0x14
    void InitEntry(SEntry_00c23e00* entry);   // 0x00C22A40
    CEventSubscription_00c23e00(unsigned long count);
};

CEventSubscription_00c23e00::CEventSubscription_00c23e00(unsigned long count)
    : m_count(count), m_8(0), m_c(0)
{
    if (count == 0) {
        m_pEntries = 0;
        m_pSlots = 0;
        return;
    }
    m_pEntries = new SEntry_00c23e00[count];
    if (!m_pEntries) {
        {
            CString_00c23e00 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c23e00();
    }
    m_pSlots = (long*)malloc(m_count * sizeof(long));
    if (!m_pSlots) {
        {
            CString_00c23e00 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(65)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c23e00();
    }
    for (unsigned long i = 0; i < m_count; ++i)
        InitEntry(&m_pEntries[i]);
}
