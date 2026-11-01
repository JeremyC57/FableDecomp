// WarnNotMorphable_00c328b0(void*) -- EH-framed `__stdcall`: a log line built on the stack from the global buffer @ 0x0139C900 (one-word holder, EH state 0 while it lives), the source file, line 24 and the warning text appended (`.\PKCTRArray.cpp(24) : Warning: This object is not morphable`) through the `__fastcall` builders (@ 0x00C0DEC0 text, @ 0x00C0DF00 number), the line handed to the optional log sink @ 0x0143BB2C, then released (@ 0x00C0BA40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C328B0, oracle length 156 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[g_buffer] ; mov [esp],eax ; mov edx,offset lit1 ; lea ecx,[esp] ; mov dword ptr [esp+0Ch],0 ; call Append ; mov edx,offset lit2 ; mov ecx,eax ; call Append ; mov edx,18h ; mov ecx,eax ; call AppendNumber ; mov edx,offset lit3 ; mov ecx,eax ; call Append ; mov edx,offset lit4 ; mov ecx,eax ; call Append ; mov edx,offset lit5 ; mov ecx,eax ; call Append ; mov eax,[g_pfnLog] ; test eax,eax ; je skip ; mov ecx,[esp] ; call eax ; skip: lea ecx,[esp] ; mov dword ptr [esp+0Ch],-1 ; call ~CLogLine ; ...
// Each builder returns the line so the calls chain through eax/ecx; the log sink is a
// `__fastcall` function pointer taking the buffer. The argument is unused (a callback shape).
// The manifest label is a description; named by the shape.
class CString { public:                 // one word: the shared buffer
    void* m_pBuffer;
    CString(void* buffer) : m_pBuffer(buffer) { }
    ~CString();                  // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
};

typedef void (__fastcall* PFNLOGSINK_00c328b0)(void* buffer);
extern void* g_logBuffer_0139c900;         // 0x0139C900
extern PFNLOGSINK_00c328b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

void __stdcall WarnNotMorphable_00c328b0(void* unused)
{
    CString line(g_logBuffer_0139c900);
    line << ".\\PKCTRArray.cpp" << "(" << 24L << ") : " << "Warning: This object is not morphable" << "\n";
    if (g_pfnLogSink_0143bb2c)
        g_pfnLogSink_0143bb2c(line.m_pBuffer);
}