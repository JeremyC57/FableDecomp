// CResourceArray_00c32bf0::~CResourceArray_00c32bf0() -- EH-framed destructor (vtable 0x012B4E80 over the base 0x012B2DB0): a non-zero count at +0xc logs `.\\PKCTRArray.cpp(59) : Error: this array collection still has items in it!` through a stack string (in its own block, state 1 over the base state 0; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and lands an empty trace temporary on a dead frame byte (@ 0x00C16750); the block at +4 is freed (`operator delete`) and nulled; the inlined base destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C32BF0, oracle length 223 bytes. Built under /EHsc.
// Retail bytes:  ... mov dword ptr [esi],012B4E80h ; mov eax,[esi+0Ch] ; test eax,eax ; mov dword ptr [esp+18h],0 ; je free ; mov eax,[g_pchNil] ; mov [esp+8],eax ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+18h],1 ; call Append ; ... AppendNumber(3Bh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+18h],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; free: mov eax,[esi+4] ; test eax,eax ; je done ; push eax ; call operator delete ; add esp,4 ; mov dword ptr [esi+4],0 ; done: mov ecx,[esp+10h] ; mov dword ptr [esi],012B2DB0h ; ...
// The manifest label is a description; the classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c32bf0)(const char* text);
extern PFNLOGSINK_00c32bf0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c32bf0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c32bf0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c32bf0();                       // 0x00C0BA40
    CString_00c32bf0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c32bf0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c32bf0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c32bf0();                         // 0x00C16750
};

void __cdecl operator delete(void* block);

struct CResourceBase_00c32bf0 {            // vtable 0x012B2DB0
    virtual void v00();
    ~CResourceBase_00c32bf0() { }
};

struct CResourceArray_00c32bf0 : CResourceBase_00c32bf0 {   // vtable 0x012B4E80
    void* m_pData;                         // +4
    long m_8;
    long m_count;                          // +0xc
    ~CResourceArray_00c32bf0();
};

CResourceArray_00c32bf0::~CResourceArray_00c32bf0()
{
    if (m_count) {
        {
            CString_00c32bf0 line;
            line.Append(".\\PKCTRArray.cpp")->Append("(")->AppendNumber(59)->Append(") : ")->Append("Error: this array collection still has items in it!")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c32bf0();
    }
    if (m_pData) {
        operator delete(m_pData);
        m_pData = 0;
    }
}
