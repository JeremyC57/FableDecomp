// CMasterObject_00c4c8b0::LoadScriptInfo(reader, a, stride, b, c, d) -- EH-framed loader of a master object's script-slave buffer (the manifest's "PKRACMasterObject_LoadScriptInfo"): under the profiling lock on +0x10 (state 0) a finalised object (+0x28) raises "Dependancy already finalised - this could be a dead object!" (.\PKRACMasterObject.cpp line 98: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750); the reader's count (vtable +8) times the stride sizes a `new` CSBAgents (0x1c bytes, built @ 0x00C62C70 over this; the raw block in the dead reader-argument slot, state 2), a failed allocation raising "EMEM" (line 105, the string in the same dead slot, state 3); the reader's load (vtable +4: a, stride, b + d, the agents' buffer @ 0x00C62C20, the agents, the global slave class object at 0x0139CA38) on failure deletes the agents (vtable +0, flag 1) and resets the object (@ 0x00C4C330), on success commits it (@ 0x00C4C6C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C8B0, oracle length 499 bytes. Built under /EHsc.
// Retail bytes: 6a ff 68 e6 5f 21 01 64 a1 00 00 00 00 50 64 89 25 00 00 00 00 83 ec 10 53 55 56 8b e9 57 8d 45 10 50 8d 4c 24 1c e8 ... (`mov al,[ebp+28h] ; xor ebx,ebx ; cmp al,bl` guard, `push 1Ch ; call operator new`, `imul esi,[esp+38h]`, `push 139CA38h ; push esi ; call 0xC62C20` then `call [ebx+4]` and `test al,al`) ...
// The class is named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c8b0)(const char* text);
extern PFNLOGSINK_00c4c8b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4c8b0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4c8b0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4c8b0();                       // 0x00C0BA40
    CString_00c4c8b0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4c8b0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c4c8b0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c4c8b0();                         // 0x00C16750
};
struct CProfileLock_00c4c8b0 {             // scoped profiling lock: two words
    void* m_pSection;
    void* m_pPrevious;
    CProfileLock_00c4c8b0(void* section);      // 0x00C0FFC0
    ~CProfileLock_00c4c8b0();                  // 0x00C0FFE0
};

struct CMasterObject_00c4c8b0;
struct CSlaveClass_00c4c8b0 { void* m_vtable; };   // the global slave class object at 0x0139CA38
extern CSlaveClass_00c4c8b0 g_slaveClass_0139ca38;

struct CSBAgents_00c4c8b0 {                // the script-slave buffer, 0x1c bytes
    virtual void Destroy(int flags);       // deleting destructor
    unsigned char _body[0x18];
    CSBAgents_00c4c8b0(CMasterObject_00c4c8b0* owner, unsigned long count, void* a, void* b, void* c);   // 0x00C62C70
    void* GetBuffer();                     // 0x00C62C20
};

struct IScriptReader_00c4c8b0 {            // the reader interface: count at +8, load at +4
    virtual void v00();
    virtual bool Load(void* a, unsigned long stride, void* end, void* buffer, CSBAgents_00c4c8b0* agents, CSlaveClass_00c4c8b0* slaveClass);
    virtual unsigned long GetCount();
};

struct CMasterObject_00c4c8b0 {
    unsigned char _pad0[0x10];
    unsigned char m_section[0x18];          // +0x10: the profiling section
    bool m_finalised;                      // +0x28
    void Reset();                          // 0x00C4C330
    void Commit();                         // 0x00C4C6C0
    void LoadScriptInfo(IScriptReader_00c4c8b0* reader, void* a, unsigned long stride, char* b, void* c, unsigned long d);
};

void CMasterObject_00c4c8b0::LoadScriptInfo(IScriptReader_00c4c8b0* reader, void* a, unsigned long stride, char* b, void* c, unsigned long d)
{
    CProfileLock_00c4c8b0 lock(m_section);
    if (m_finalised) {
        {
            CString_00c4c8b0 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(98)->Append(") : ")->Append("Dependancy already finalised - this could be a dead object!")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4c8b0();
    }
    unsigned long count = reader->GetCount();
    CSBAgents_00c4c8b0* agents = new CSBAgents_00c4c8b0(this, count * stride, (void*)d, b, c);
    if (!agents) {
        {
            CString_00c4c8b0 line;
            line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(105)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4c8b0();
    }
    if (!reader->Load(a, stride, b + d, agents->GetBuffer(), agents, &g_slaveClass_0139ca38)) {
        if (agents)
            agents->Destroy(1);
        Reset();
    } else {
        Commit();
    }
}