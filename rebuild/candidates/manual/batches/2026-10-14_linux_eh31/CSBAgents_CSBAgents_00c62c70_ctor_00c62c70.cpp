// CSBAgents_00c62c70::CSBAgents(owner, count, a, b, c) -- EH-framed constructor of a local slave-object buffer (vtable 0x012B938C over the base built from the owner @ 0x00C62E20, whose inline empty destructor stores 0x012B93D4 (0x00C62E10 on unwind; state 0)): the count at +8, the two qualifiers at +0xc / +0x10, the buffer pointer at +0x14 cleared and the fifth argument at +0x18; a zero count raises "Expecting to be reading some data!" (PKRASlaveObject_Local.cpp line 15) and a failed `operator new` of the count bytes "EMEM" (line 17), each an MFC-style string in a nested block handed to the log sink at 0x0143BB2C then the empty trace temporary @ 0x00C16750, both homed in the dead owner-argument slot (states 1 and 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62C70, oracle length 378 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; mov eax,[esp+14h] ; push ebx ; push esi ; push edi ; mov esi,ecx ; push eax ; mov [esp+10h],esi ; call CSlaveObject::CSlaveObject ; mov edi,[esp+24h] ; mov ecx,[esp+28h] ; mov edx,[esp+2Ch] ; mov eax,[esp+30h] ; xor ebx,ebx ; cmp edi,ebx ; mov [esp+18h],ebx ; mov dword ptr [esi],12B938Ch ; mov [esi+8],edi ; mov [esi+0Ch],ecx ; mov [esi+10h],edx ; mov [esi+14h],ebx ; mov [esi+18h],eax ; ja alloc ; mov ecx,[g_pchNil] ; mov [esp+20h],ecx ; mov edx,offset ".\PKRASlaveObject_Local.cpp" ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],1 ; call Append ; ... mov edx,0Fh ; ... call AppendNumber ; ... call Append ("Expecting to be reading some data!") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; cmp eax,ebx ; je skip ; mov ecx,[esp+20h] ; call eax ; skip: lea ecx,[esp+20h] ; mov [esp+18h],bl ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; alloc: push edi ; call operator new ; add esp,4 ; cmp eax,ebx ; mov [esi+14h],eax ; jne done ; (the same warning with 11h and "EMEM", state 2) ; done: ... mov eax,esi ; ... ret 14h
// The classes are named by address.
void* __cdecl operator new(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c62c70)(const char* text);
extern PFNLOGSINK_00c62c70 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c62c70 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c62c70() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c62c70();                       // 0x00C0BA40
    CString_00c62c70* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c62c70* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c62c70 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c62c70();                         // 0x00C16750
};

struct CSlaveObject_00c62c70 {             // vtable 0x012B93D4: the base, 8 bytes
    virtual ~CSlaveObject_00c62c70() { }
    void* m_pOwner;                        // +4
    CSlaveObject_00c62c70(void* owner);    // 0x00C62E20
};

struct CSBAgents_00c62c70 : CSlaveObject_00c62c70 {
    unsigned long m_count;                 // +8
    void* m_c;
    void* m_10;
    unsigned char* m_pBuffer;              // +0x14
    void* m_18;
    CSBAgents_00c62c70(void* owner, unsigned long count, void* a, void* b, void* c);
    virtual ~CSBAgents_00c62c70();
};

CSBAgents_00c62c70::CSBAgents_00c62c70(void* owner, unsigned long count, void* a, void* b, void* c)
    : CSlaveObject_00c62c70(owner), m_count(count), m_c(a), m_10(b), m_pBuffer(0), m_18(c)
{
    if (!(count > 0)) {
        {
            CString_00c62c70 line;
            line.Append(".\\PKRASlaveObject_Local.cpp")->Append("(")->AppendNumber(15)->Append(") : ")->Append("Expecting to be reading some data!")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c62c70();
    }
    m_pBuffer = new unsigned char[count];
    if (!m_pBuffer) {
        {
            CString_00c62c70 line;
            line.Append(".\\PKRASlaveObject_Local.cpp")->Append("(")->AppendNumber(17)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c62c70();
    }
}
