// CEventSubscription_00c239b0::CEventSubscription(count) -- EH-framed constructor (vtable 0x012B3A94; the manifest's "NScript_CEvent_InitializeSubscriptions"; the landed 0x00C23C10 is the plain-`malloc` twin): the count at +4, +8 / +0xc cleared; a zero count leaves the two tables at +0x10 / +0x14 empty; otherwise a 180-byte-per-entry table is built with `new[]` over the class allocator (`malloc`, the raw block freed on unwind; `__ehvec_ctor` with 0x00C21BA0 / 0x00C207F0) and a word-per-entry table `malloc`ed, each failure raising "EMEM"-style warnings ("Memory allocation failure", d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h lines 63 and 65: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 -- the first string and trace in the dead argument slot, the second trace on a free frame byte; states 0 and 1), and every entry is initialised (@ 0x00C22840)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C239B0, oracle length 489 bytes. Built under /EHsc.
// Retail bytes:  ... (as 0x00C23C10, the entry table from `imul eax,eax,0B4h ; add eax,4 ; push eax ; call malloc ; ... mov [eax],edi ; call __ehvec_ctor` with 0x00C21BA0 / 0x00C207F0, the block freed on unwind; the entries initialised @ 0x00C22840 every 0B4h bytes) ...
// The class is named by address.
void* __cdecl malloc(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c239b0)(const char* text);
extern PFNLOGSINK_00c239b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c239b0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c239b0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c239b0();                       // 0x00C0BA40
    CString_00c239b0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c239b0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c239b0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c239b0();                         // 0x00C16750
};

void __cdecl free(void* block);
struct SEntry_00c239b0 {                    // 180 bytes per entry, built and torn down out of line
    unsigned char _body[0xb4];
    SEntry_00c239b0();                           // 0x00C21BA0
    ~SEntry_00c239b0();                          // 0x00C207F0
    static void* operator new[](unsigned int size) { return malloc(size); }
    static void operator delete[](void* block) { free(block); }
};

struct CEventSubscription_00c239b0 {
    virtual void v00();
    unsigned long m_count;                 // +4
    long m_8;
    long m_c;
    SEntry_00c239b0* m_pEntries;           // +0x10
    long* m_pSlots;                        // +0x14
    void InitEntry(SEntry_00c239b0* entry);   // 0x00C22840
    CEventSubscription_00c239b0(unsigned long count);
};

CEventSubscription_00c239b0::CEventSubscription_00c239b0(unsigned long count)
    : m_count(count), m_8(0), m_c(0)
{
    if (count == 0) {
        m_pEntries = 0;
        m_pSlots = 0;
        return;
    }
    m_pEntries = new SEntry_00c239b0[count];
    if (!m_pEntries) {
        {
            CString_00c239b0 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c239b0();
    }
    m_pSlots = (long*)malloc(m_count * sizeof(long));
    if (!m_pSlots) {
        {
            CString_00c239b0 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(65)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c239b0();
    }
    for (unsigned long i = 0; i < m_count; ++i)
        InitEntry(&m_pEntries[i]);
}