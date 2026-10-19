// CEventSubscription_00c23c10::CEventSubscription(count) -- EH-framed constructor (vtable 0x012B3820; the manifest's "NScript_InitializeEventSubscription"): the count at +4, +8 / +0xc cleared; a zero count leaves the two tables at +0x10 / +0x14 empty; otherwise a 20-byte-per-entry table and a word-per-entry table are allocated with `new long[]` (operator new[] @ 0x00BFEB22), each failure raising "EMEM"-style warnings ("Memory allocation failure", d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h lines 63 and 65: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 -- the first string and trace in the dead argument slot, the second trace on a free frame byte; states 0 and 1), and every entry is initialised (@ 0x00C22920)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C23C10, oracle length 436 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; mov eax,[esp+14h] ; push esi ; mov esi,ecx ; push edi ; xor edi,edi ; cmp eax,edi ; mov dword ptr [esi],12B3820h ; mov [esi+4],eax ; mov [esi+8],edi ; mov [esi+0Ch],edi ; jne alloc ; mov [esi+10h],edi ; mov [esi+14h],edi ; ... mov eax,esi ; ... ret 4 ; alloc: lea eax,[eax+eax*4] ; shl eax,2 ; push ebx ; push eax ; call operator new[] ; add esp,4 ; or ebx,-1 ; cmp eax,edi ; mov [esi+10h],eax ; jne second ; mov ecx,[g_pchNil] ; mov [esp+20h],ecx ; mov edx,offset file ; lea ecx,[esp+20h] ; mov [esp+18h],edi ; call Append ; ... mov edx,3Fh ; ... call AppendNumber ; ... call Append (text) ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; cmp eax,edi ; je a ; mov ecx,[esp+20h] ; call eax ; a: lea ecx,[esp+20h] ; mov [esp+18h],ebx ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; second: mov edx,[esi+4] ; shl edx,2 ; push edx ; call operator new[] ; add esp,4 ; cmp eax,edi ; mov [esi+14h],eax ; jne init ; (the same warning with 41h, state 1, trace at [esp+0Fh]) ; init: cmp [esi+4],edi ; jbe done ; xor ebx,ebx ; loop: mov ecx,[esi+10h] ; add ecx,ebx ; push ecx ; mov ecx,esi ; call CEventSubscription::InitEntry ; mov eax,[esi+4] ; inc edi ; add ebx,14h ; cmp edi,eax ; jb loop ; done: ... mov eax,esi ; ... ret 4
// The class is named by address.
// Both tables come from the global array allocator: retail calls MSVCR71 operator new[] (0x00BFEB22) for each.
void* __cdecl operator new[](unsigned int size);     // 0x00BFEB22 (the lane <new> lacks it)
void __cdecl operator delete[](void* block);        // 0x00BFEB1C
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c23c10)(const char* text);
extern PFNLOGSINK_00c23c10 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c23c10 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c23c10() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c23c10();                       // 0x00C0BA40
    CString_00c23c10* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c23c10* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c23c10 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c23c10();                         // 0x00C16750
};

struct SEntry_00c23c10 { unsigned char _body[0x14]; };   // 20 bytes per subscription entry

struct CEventSubscription_00c23c10 {
    virtual void v00();
    unsigned long m_count;                 // +4
    long m_8;
    long m_c;
    SEntry_00c23c10* m_pEntries;           // +0x10
    long* m_pSlots;                        // +0x14
    void InitEntry(SEntry_00c23c10* entry);   // 0x00C22920
    CEventSubscription_00c23c10(unsigned long count);
};

CEventSubscription_00c23c10::CEventSubscription_00c23c10(unsigned long count)
    : m_count(count), m_8(0), m_c(0)
{
    if (count == 0) {
        m_pEntries = 0;
        m_pSlots = 0;
        return;
    }
    m_pEntries = new SEntry_00c23c10[count];
    if (!m_pEntries) {
        {
            CString_00c23c10 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c23c10();
    }
    m_pSlots = new long[m_count];
    if (!m_pSlots) {
        {
            CString_00c23c10 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(65)->Append(") : ")->Append("Memory allocation failure")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c23c10();
    }
    for (unsigned long i = 0; i < m_count; ++i)
        InitEntry(&m_pEntries[i]);
}