// CDependency_00c4c6c0::OnFinalized() -- EH-framed: under a scoped lock over the member at +0x10 (state 0), when the flag at +0x28 is set a stack string logs `.\\PKRACMasterObject.cpp(71) : Dependancy finalised... this could be a dead object...` (in its own block, state 1; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and an empty trace temporary lands on a dead frame byte (@ 0x00C16750); the counter at +0 is incremented
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C6C0, oracle length 220 bytes. Built under /EHsc.
// Retail bytes:  ... lea eax,[esi+10h] ; push eax ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov al,[esi+28h] ; test al,al ; mov dword ptr [esp+1Ch],0 ; je bump ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... AppendNumber(47h) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; bump: mov eax,[esi] ; inc eax ; lea ecx,[esp+0Ch] ; mov [esi],eax ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; ...
// The lock target is the member itself. The manifest label is a description; the class is
// named by address.
class PKCCriticalSection;
struct CLockTarget_00c4c6c0;
class PKCProtectionInstance { public:              // 8-byte scoped lock
    CLockTarget_00c4c6c0* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c6c0)(const char* text);
extern PFNLOGSINK_00c4c6c0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4c6c0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4c6c0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4c6c0();                       // 0x00C0BA40
    CString_00c4c6c0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4c6c0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c4c6c0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c4c6c0();                         // 0x00C16750
};

struct CLockTarget_00c4c6c0 {
    unsigned char _body[0x18];
};

struct CDependency_00c4c6c0 {
    long m_count;                          // +0
    unsigned char _pad[0xc];
    CLockTarget_00c4c6c0 m_lock;               // +0x10
    bool m_finalised;                      // +0x28
    void OnFinalized();
};

void CDependency_00c4c6c0::OnFinalized()
{
    PKCProtectionInstance lock((PKCCriticalSection*)&m_lock);
    if (m_finalised) {
        {
            CString_00c4c6c0 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(71)->Append(") : ")->Append("Dependancy finalised... this could be a dead object...")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4c6c0();
    }
    ++m_count;
}