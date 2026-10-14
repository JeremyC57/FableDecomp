// CAsyncDecoder_00c48640::CAsyncDecoder() -- EH-framed default constructor (the manifest's "CScriptDefinitionManager_Initialize" is propagated): the decoder base is built first (@ 0x00C485E0; destructor @ 0x00C48220, state 0), the thread pointer at +0x38 cleared, an 8-byte thread parameter record {0x3000, 0} built on the frame and a 12-byte worker thread allocated from the decoder and the record (null-checked `new`, state 1 frees it if the constructor @ 0x00C355E0 throws); a null thread raises the "EMEM" warning (CASyncDecoder.cpp line 193; state 2 covers the string, the trace temporary lands on a free frame byte)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48640, oracle length 254 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,14h ; push ebx ; push esi ; mov esi,ecx ; mov [esp+10h],esi ; call CDecoderBase::CDecoderBase ; xor ebx,ebx ; push 0Ch ; mov [esp+28h],ebx ; mov [esi+38h],ebx ; mov [esp+1Ch],ebx ; mov dword ptr [esp+18h],3000h ; call operator new ; add esp,4 ; mov [esp+0Ch],eax ; cmp eax,ebx ; mov byte ptr [esp+24h],1 ; je null ; lea ecx,[esp+14h] ; push ecx ; push esi ; mov ecx,eax ; call CWorkerThread::CWorkerThread ; jmp got ; null: xor eax,eax ; got: cmp eax,ebx ; mov byte ptr [esp+24h],bl ; mov [esi+38h],eax ; jne done ; mov edx,[g_pchNil] ; mov [esp+0Ch],edx ; mov edx,offset ".\CASyncDecoder.cpp" ; lea ecx,[esp+0Ch] ; mov byte ptr [esp+24h],2 ; call Append ; ... mov edx,0C1h ; ... call AppendNumber ; ... call Append ("EMEM") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; cmp eax,ebx ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov byte ptr [esp+24h],bl ; call ~CString ; lea ecx,[esp+0Bh] ; call CTrace::CTrace ; done: ... mov eax,esi ; ... ret
// The classes are named by address.
void* __cdecl operator new(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c48640)(const char* text);
extern PFNLOGSINK_00c48640 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c48640 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c48640() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c48640();                       // 0x00C0BA40
    CString_00c48640* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c48640* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c48640 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c48640();                         // 0x00C16750
};

struct SThreadParams_00c48640 {            // 8 bytes: stack size and flags
    long m_stackSize;
    long m_flags;
};

struct CAsyncDecoder_00c48640;
struct CWorkerThread_00c48640 {            // 12 bytes
    unsigned char _body[0xc];
    CWorkerThread_00c48640(CAsyncDecoder_00c48640* owner, const SThreadParams_00c48640* params);   // 0x00C355E0
};

struct CDecoderBase_00c48640 {             // 0x38 bytes
    unsigned char _body[0x38];
    CDecoderBase_00c48640();               // 0x00C485E0
    ~CDecoderBase_00c48640();              // 0x00C48220
};

struct CAsyncDecoder_00c48640 : CDecoderBase_00c48640 {
    CWorkerThread_00c48640* m_pThread;     // +0x38
    CAsyncDecoder_00c48640();
};

CAsyncDecoder_00c48640::CAsyncDecoder_00c48640()
    : CDecoderBase_00c48640(), m_pThread(0)
{
    SThreadParams_00c48640 params;
    params.m_flags = 0;                    // retail stores the flags before the stack size
    params.m_stackSize = 0x3000;
    m_pThread = new CWorkerThread_00c48640(this, &params);
    if (m_pThread == 0) {
        {
            CString_00c48640 line;
            line.Append(".\\CASyncDecoder.cpp")->Append("(")->AppendNumber(193)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c48640();
    }
}