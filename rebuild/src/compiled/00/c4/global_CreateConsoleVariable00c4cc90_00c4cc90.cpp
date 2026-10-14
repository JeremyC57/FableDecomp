// CreateConsoleVariable_00c4cc90(void*, void*) -- EH-framed `__fastcall` factory: a 0x30-byte variable is allocated (`new`, state 0) and built with the two register arguments (@ 0x00C4C2F0); a failed allocation logs `.\\PKRACMasterObject.cpp(146) : EMEM` through a stack string (in its own block, state 1; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and lands an empty trace temporary on a dead frame byte (@ 0x00C16750); returns the variable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4CC90, oracle length 233 bytes. Built under /EHsc.
// Retail bytes:  ... push 30h ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; mov [esp+0Ch],eax ; test eax,eax ; mov dword ptr [esp+18h],0 ; je null ; push esi ; push edi ; mov ecx,eax ; call CConsoleVariable::CConsoleVariable ; mov esi,eax ; jmp built ; null: xor esi,esi ; built: or edi,-1 ; test esi,esi ; mov [esp+18h],edi ; jne done ; mov eax,[g_pchNil] ; mov [esp+0Ch],eax ; mov edx,offset file ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],1 ; call Append ; ... AppendNumber(92h) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+0Ch] ; call eax ; rel: lea ecx,[esp+0Ch] ; mov [esp+18h],edi ; call ~CString ; lea ecx,[esp+0Bh] ; call CTrace::CTrace ; done: ... mov eax,esi ... ret
// The -1 state is materialised in edi for its two stores; the string reuses the `new`
// temporary's slot. The manifest label is a description; named by the shape.
struct CConsoleVariable_00c4cc90 {         // 0x30 bytes
    unsigned char _body[0x30];
    CConsoleVariable_00c4cc90(void* name, void* value);   // 0x00C4C2F0
};

extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4cc90)(const char* text);
extern PFNLOGSINK_00c4cc90 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4cc90 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4cc90() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4cc90();                       // 0x00C0BA40
    CString_00c4cc90* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4cc90* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c4cc90 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c4cc90();                         // 0x00C16750
};

CConsoleVariable_00c4cc90* __fastcall CreateConsoleVariable_00c4cc90(void* name, void* value)
{
    CConsoleVariable_00c4cc90* variable = new CConsoleVariable_00c4cc90(name, value);
    if (!variable) {
        {
            CString_00c4cc90 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(146)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4cc90();
    }
    return variable;
}