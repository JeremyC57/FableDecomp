// CMasterObject_00c4c7a0::ReceiveObject(late) -- EH-framed under the object's own profiling lock on +0x10 (@ 0x00C0FFC0 / 0x00C0FFE0, state 0): receiving past the expected count (+4 not below +0) raises the warning at PKRACMasterObject.cpp line 79 (MFC-style string in a nested block, log sink at 0x0143BB2C, the trace temporary on a free frame byte; state 1); an on-time object bumps the +8 counter; the received count is incremented and, when it reaches the expected count of an already finalised object (+0x28), the object completes (@ 0x00C4C440)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C7A0, oracle length 257 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,10h ; push esi ; mov esi,ecx ; lea eax,[esi+10h] ; push eax ; lea ecx,[esp+10h] ; call CProfileLock::CProfileLock ; mov ecx,[esi+4] ; cmp ecx,[esi] ; mov dword ptr [esp+1Ch],0 ; jb ok ; mov edx,[g_pchNil] ; mov [esp+8],edx ; mov edx,offset ".\PKRACMasterObject.cpp" ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... mov edx,4Fh ; ... call AppendNumber ; ... call Append (text) ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; ok: mov al,[esp+24h] ; test al,al ; jne a ; inc dword ptr [esi+8] ; a: mov edx,[esi+4] ; mov ecx,[esi] ; inc edx ; mov eax,edx ; cmp ecx,eax ; mov [esi+4],edx ; jne b ; mov al,[esi+28h] ; test al,al ; je b ; mov ecx,esi ; call Complete ; b: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CProfileLock ; ... ret 4
// The sibling destructor landed at 0x00C4C370 and the finalise hook at 0x00C4C6C0. The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c7a0)(const char* text);
extern PFNLOGSINK_00c4c7a0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4c7a0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4c7a0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4c7a0();                       // 0x00C0BA40
    CString_00c4c7a0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4c7a0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c4c7a0 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c4c7a0();                         // 0x00C16750
};

struct CProfileLock_00c4c7a0 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c4c7a0(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c4c7a0();              // 0x00C0FFE0
};

struct CMasterObject_00c4c7a0 {
    unsigned long m_expected;              // +0
    unsigned long m_received;              // +4
    long m_onTime;                         // +8
    long m_c;
    unsigned char m_section[0x18];         // +0x10
    bool m_finalised;                      // +0x28
    void Complete();                       // 0x00C4C440
    void ReceiveObject(bool late);
};

void CMasterObject_00c4c7a0::ReceiveObject(bool late)
{
    CProfileLock_00c4c7a0 lock(m_section);
    if (!(m_received < m_expected)) {
        {
            CString_00c4c7a0 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(79)->Append(") : ")->Append("No room for this received object... sniff sniff")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4c7a0();
    }
    if (!late)
        ++m_onTime;
    if (m_expected == ++m_received) {
        if (m_finalised)
            Complete();
    }
}
