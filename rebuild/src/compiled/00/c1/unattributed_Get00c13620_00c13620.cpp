// CEntityArray_00c13620::Get(unsigned long) -- EH-framed: an index at or beyond the count at +4 logs `d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h(31) : Error: Fralloc overflow` through a stack log line (shared buffer @ 0x0139C900, state 0, in its own block, chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and builds an empty trace temporary in the dead argument slot (@ 0x00C16750); returns the 0xb4-byte element at the index of the block at +0x10 regardless
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13620, oracle length 205 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+4] ; push edi ; mov edi,[esp+1Ch] ; cmp edi,eax ; jb ok ; mov eax,[g_logBuffer] ; mov [esp+8],eax ; mov edx,offset file ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call Append ; ... AppendNumber(1Fh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CLogLine ; lea ecx,[esp+1Ch] ; call CTrace::CTrace ; ok: mov ecx,[esi+10h] ; mov eax,edi ; imul eax,eax,0B4h ; add eax,ecx ; ... ret 4
// The accessor every inventory walk calls (0x00C13620). The manifest label is a description;
// the class is named by address.
struct CLogLine_00c13620 {                 // one word: the shared buffer
    void* m_pBuffer;
    CLogLine_00c13620(void* buffer) : m_pBuffer(buffer) { }
    ~CLogLine_00c13620();                      // 0x00C0BA40
    CLogLine_00c13620* __fastcall Append(const char* text);   // 0x00C0DEC0
    CLogLine_00c13620* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

typedef void (__fastcall* PFNLOGSINK_00c13620)(void* buffer);
extern void* g_logBuffer_0139c900;         // 0x0139C900
extern PFNLOGSINK_00c13620 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CTrace_00c13620 {                       // empty temporary (a debug-break helper): homed in a dead byte of the frame
    CTrace_00c13620();                         // 0x00C16750: calls an import and returns `this`
};

struct CEntity_00c13620 {                  // 0xb4 bytes
    unsigned char _body[0xb4];
};

struct CEntityArray_00c13620 {
    long m_0;
    unsigned long m_count;                 // +4
    long m_8;
    long m_c;
    CEntity_00c13620* m_pData;                 // +0x10
    CEntity_00c13620* Get(unsigned long index);
};

CEntity_00c13620* CEntityArray_00c13620::Get(unsigned long index)
{
    if (index >= m_count) {
        {
            CLogLine_00c13620 line(g_logBuffer_0139c900);
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(31)->Append(") : ")->Append("Error: Fralloc overflow")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pBuffer);
        }
        CTrace_00c13620();
    }
    return &m_pData[index];
}