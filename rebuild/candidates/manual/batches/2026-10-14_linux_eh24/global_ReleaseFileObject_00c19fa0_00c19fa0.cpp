// ReleaseFileObject_00c19fa0(CFileObject*) -- EH-framed `__stdcall`: when the file object is null a log line is built on the stack from the shared buffer @ 0x0139C900 (state 0, in its own block: `.\PKCDefaultFileManager.cpp(91) : NULL File Object`, chained `__fastcall` builders, the optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and an empty trace temporary built in the dead argument slot (@ 0x00C16750); else the object's virtual slot 4 runs with 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19FA0, oracle length 199 bytes. Built under /EHsc.
// Retail bytes:  ... mov ecx,[esp+14h] ; test ecx,ecx ; jne rel ; mov eax,[g_logBuffer] ; mov [esp],eax ; mov [esp+0Ch],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp] ; call Append ; ... AppendNumber(5Bh) ... "NULL File Object" ... "\n" ; mov eax,[g_pfnLogSink] ; test eax,eax ; je done ; mov ecx,[esp] ; call eax ; done: lea ecx,[esp] ; mov dword ptr [esp+0Ch],-1 ; call ~CLogLine ; lea ecx,[esp+14h] ; call CTrace::CTrace ; ... ret 4 ; rel: mov edx,[ecx] ; push 1 ; call [edx+10h] ; ... ret 4
// The trace temporary lives only in the null branch here. The manifest label is a description;
// named by the shape.
struct CLogLine_00c19fa0 {                 // one word: the shared buffer
    void* m_pBuffer;
    CLogLine_00c19fa0(void* buffer) : m_pBuffer(buffer) { }
    ~CLogLine_00c19fa0();                      // 0x00C0BA40
    CLogLine_00c19fa0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CLogLine_00c19fa0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c19fa0 {                       // empty temporary (a debug-break helper): homed in a dead byte (here the argument slot), so the log line keeps a frame local
    CTrace_00c19fa0();                         // 0x00C16750: calls an import and returns `this`
};

typedef void (__fastcall* PFNLOGSINK_00c19fa0)(void* buffer);
extern void* g_logBuffer_0139c900;         // 0x0139C900
extern PFNLOGSINK_00c19fa0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C


struct CFileObject_00c19fa0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Release(long flags);      // slot 4
};

void __stdcall ReleaseFileObject_00c19fa0(CFileObject_00c19fa0* file)
{
    if (!file) {
        {
            CLogLine_00c19fa0 line(g_logBuffer_0139c900);
            line.Append(".\\PKCDefaultFileManager.cpp")->Append("(")->AppendNumber(91)->Append(") : ")->Append("NULL File Object")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pBuffer);
        }
        CTrace_00c19fa0();
        return;
    }
    file->Release(1);
}
