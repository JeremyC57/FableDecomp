// CMasterObject_00c4c370::~CMasterObject_00c4c370() -- EH-framed destructor: when the finalised flag at +0x28 is clear a log line is built on the stack (`.\\PKRACMasterObject.cpp(8) : If it was not finalised, then how did I get here?`, state 1 over the member state 0, chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and an empty trace temporary built in a dead frame byte (@ 0x00C16750); then the member at +0x10 is destroyed out of line (@ 0x00C1C820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C370, oracle length 204 bytes. Built under /EHsc.
// Retail bytes:  ... mov al,[esi+28h] ; test al,al ; mov dword ptr [esp+18h],0 ; jne done ; mov eax,[g_logBuffer] ; mov [esp+8],eax ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+18h],1 ; call Append ; ... AppendNumber(8) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+18h],0 ; call ~CLogLine ; lea ecx,[esp+7] ; call CTrace::CTrace ; done: lea ecx,[esi+10h] ; mov dword ptr [esp+18h],-1 ; call ~CMember ; ...
// The empty trace temporary lands on the high byte of a frame slot (a named local gets its own slot under the DDK). The class stores no vtable of
// its own. The manifest label is a description; the class is named by address.
struct CLogLine_00c4c370 {                 // one word: the shared buffer
    void* m_pBuffer;
    CLogLine_00c4c370(void* buffer) : m_pBuffer(buffer) { }
    ~CLogLine_00c4c370();                      // 0x00C0BA40
    CLogLine_00c4c370* __fastcall Append(const char* text);   // 0x00C0DEC0
    CLogLine_00c4c370* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

typedef void (__fastcall* PFNLOGSINK_00c4c370)(void* buffer);
extern void* g_logBuffer_0139c900;         // 0x0139C900
extern PFNLOGSINK_00c4c370 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CTrace_00c4c370 {                       // empty temporary (a debug-break helper): homed in a dead byte of the frame
    CTrace_00c4c370();                         // 0x00C16750: calls an import and returns `this`
};

struct CMember_00c4c370 {                  // +0x10
    unsigned char _body[0x18];
    ~CMember_00c4c370();                       // 0x00C1C820
};

struct CMasterObject_00c4c370 {
    unsigned char _pad[0x10];
    CMember_00c4c370 m_10;                     // +0x10
    bool m_finalised;                      // +0x28
    ~CMasterObject_00c4c370();
};

CMasterObject_00c4c370::~CMasterObject_00c4c370()
{
    if (!m_finalised) {
        {
            CLogLine_00c4c370 line(g_logBuffer_0139c900);
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(8)->Append(") : ")->Append("If it was not finalised, then how did I get here?")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pBuffer);
        }
        CTrace_00c4c370();
    }
}