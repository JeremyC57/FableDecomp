// CBuffer_00c4c1d0::CheckAlignmentOffset(bytes) -- EH-framed: a byte request that is not a multiple of the alignment at +0xc raises the "your byte request (N) was not aligned to M" warning (PKRACBuffer.cpp line 81: an MFC-style string appended in a nested block with the unsigned `AppendNumber` overload @ 0x00C0DF10, handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot); the request divided by the alignment (re-read) is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C1D0, oracle length 237 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,[esp+20h] ; mov ebx,ecx ; mov esi,[ebx+0Ch] ; xor edx,edx ; mov eax,edi ; div esi ; test edx,edx ; je ok ; mov eax,[g_pchNil] ; mov [esp+0Ch],eax ; mov edx,offset ".\PKRACBuffer.cpp" ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],0 ; call Append ; mov edx,offset "(" ; ... call Append ; mov edx,51h ; ... call AppendNumber ; mov edx,offset ") : " ; ... call Append ; mov edx,offset "your byte request (" ; ... call Append ; mov edx,edi ; mov ecx,eax ; call AppendNumber(unsigned) ; mov edx,offset ") was not aligned to " ; ... call Append ; mov edx,esi ; mov ecx,eax ; call AppendNumber(unsigned) ; mov edx,offset "\n" ; ... call Append ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],-1 ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; ok: mov ecx,[esp+10h] ; mov eax,edi ; pop edi ; xor edx,edx ; div dword ptr [ebx+0Ch] ; ... ret 4
// The sibling callers at 0x00C4C2C0 / 0x00C4C2D0 pass the free and used extents. The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c1d0)(const char* text);
extern PFNLOGSINK_00c4c1d0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                       // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);   // 0x00C0DF10
};

struct CDebugBreak {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CDebugBreak();                         // 0x00C16750
};

struct CBuffer_00c4c1d0 {
    long m_0;
    long m_4;
    long m_8;
    unsigned long m_alignment;             // +0xc
    unsigned long CheckAlignmentOffset(unsigned long bytes);
};

unsigned long CBuffer_00c4c1d0::CheckAlignmentOffset(unsigned long bytes)
{
    unsigned long alignment = m_alignment;
    if (bytes % alignment != 0) {
        {
            CString line;
            line << ".\\PKRACBuffer.cpp" << "(" << 81L << ") : " << "your byte request (" << bytes << ") was not aligned to " << alignment << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    return bytes / m_alignment;
}