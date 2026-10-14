// CMasterObject_00c4cd80::Finalize() -- EH-framed under the object's own profiling lock on +0x10 (@ 0x00C0FFC0 / 0x00C0FFE0, state 0; the manifest's "PKRACMasterObject_Finalize"): an object already finalised (byte +0x28) raises the "Dependancy already finalised - this could be a dead object!" warning (PKRACMasterObject.cpp line 41: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 on a free frame byte; state 1 covers the string); the flag is set; with no dependants left at +0 the object destroys itself (destructor @ 0x00C4C370 then `operator delete`) and false is returned; when the dependant count equals the +4 target the completion @ 0x00C4C440 runs; true is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4CD80, oracle length 290 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,10h ; push esi ; mov esi,ecx ; lea eax,[esi+10h] ; push eax ; lea ecx,[esp+10h] ; call CProfileLock::CProfileLock ; mov al,[esi+28h] ; test al,al ; mov dword ptr [esp+1Ch],0 ; je set ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset ".\PKRACMasterObject.cpp" ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... mov edx,29h ; ... call AppendNumber ; ... call Append ("Dependancy already finalised - this could be a dead object!") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; set: mov eax,[esi] ; test eax,eax ; mov byte ptr [esi+28h],1 ; jne alive ; mov ecx,esi ; call CMasterObject::~CMasterObject ; push esi ; call operator delete ; add esp,4 ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CProfileLock ; xor al,al ; ... ret ; alive: cmp eax,[esi+4] ; jne done ; mov ecx,esi ; call CMasterObject::Complete ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CProfileLock ; ... mov al,1 ; ... ret
// The classes are named by address.
void __cdecl operator delete(void* block);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4cd80)(const char* text);
extern PFNLOGSINK_00c4cd80 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4cd80 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4cd80() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4cd80();                       // 0x00C0BA40
    CString_00c4cd80* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4cd80* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c4cd80 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c4cd80();                         // 0x00C16750
};

struct CProfileLock_00c4cd80 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c4cd80(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c4cd80();              // 0x00C0FFE0
};

struct CMasterObject_00c4cd80 {
    long m_dependants;                     // +0
    long m_target;                         // +4
    long m_8;
    long m_c;
    unsigned char m_section[0x18];         // +0x10
    bool m_bFinalised;                     // +0x28
    ~CMasterObject_00c4cd80();             // 0x00C4C370
    void Complete();                       // 0x00C4C440
    bool Finalize();
};

bool CMasterObject_00c4cd80::Finalize()
{
    CProfileLock_00c4cd80 lock(m_section);
    if (m_bFinalised) {
        {
            CString_00c4cd80 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(41)->Append(") : ")->Append("Dependancy already finalised - this could be a dead object!")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4cd80();
    }
    m_bFinalised = true;
    if (m_dependants == 0) {
        delete this;
        return false;
    }
    if (m_dependants == m_target)
        Complete();
    return true;
}