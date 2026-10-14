// CMailbox_00c1db60::ReplyToUser(char, void*) -- EH-framed: an 8-byte reply record {flag, message} is built on the stack and handed to the queue at +4 (@ 0x00C1DC40); a non-zero result logs `d:\dev\audio\ver_fable_1.5_b\libpk\PKCASyncFileReader.h(41) : Cannot reply to the user mailbox` through a stack string (default-constructed from the shared empty data @ 0x0139C900, state 0, in its own block, chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and builds an empty trace temporary in the dead first-argument slot (@ 0x00C16750)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1DB60, oracle length 209 bytes. Built under /EHsc.
// Retail bytes:  ... mov al,[esp+18h] ; mov edx,[esp+1Ch] ; mov ecx,[ecx+4] ; mov [esp],al ; lea eax,[esp] ; push eax ; mov [esp+8],edx ; call CMailboxQueue::Post ; test eax,eax ; je done ; mov ecx,[g_pchNil] ; mov [esp+1Ch],ecx ; mov edx,offset file ; lea ecx,[esp+1Ch] ; mov dword ptr [esp+10h],0 ; call Append ; ... AppendNumber(29h) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+1Ch] ; call eax ; rel: lea ecx,[esp+1Ch] ; mov dword ptr [esp+10h],-1 ; call ~CString ; lea ecx,[esp+18h] ; call CTrace::CTrace ; done: ... ret 8
// The string local is homed in the dead second-argument slot and the trace temporary in the
// first. The manifest label is a description; the class is named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c1db60)(const char* text);
extern PFNLOGSINK_00c1db60 g_pfnLogSink_0143bb2c;   // 0x0143BB2C
struct CString_00c1db60 {                  // MFC-style string: one word, the shared empty data @ 0x0139C900 for the default constructor
    char* m_pchData;
    CString_00c1db60() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c1db60();                       // 0x00C0BA40
    CString_00c1db60* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c1db60* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c1db60 {                   // empty temporary (a debug-break helper)
    CTrace_00c1db60();                         // 0x00C16750
};

struct SReply_00c1db60 {
    char m_flag;
    void* m_pMessage;
};

struct CMailboxQueue_00c1db60 {
    long Post(SReply_00c1db60* reply);         // 0x00C1DC40
};

struct CMailbox_00c1db60 {
    long m_0;
    CMailboxQueue_00c1db60* m_pQueue;          // +4
    void ReplyToUser(char flag, void* message);
};

void CMailbox_00c1db60::ReplyToUser(char flag, void* message)
{
    SReply_00c1db60 reply;
    reply.m_flag = flag;
    reply.m_pMessage = message;
    if (m_pQueue->Post(&reply)) {
        {
            CString_00c1db60 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCASyncFileReader.h")->Append("(")->AppendNumber(41)->Append(") : ")->Append("Cannot reply to the user mailbox")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c1db60();
    }
}