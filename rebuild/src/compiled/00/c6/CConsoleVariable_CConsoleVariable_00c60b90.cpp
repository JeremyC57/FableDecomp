// CConsoleVariable::~CConsoleVariable() -- EH-framed destructor: unregisters itself from the registry at +0x14, then the base destructor (inline, restores the base vtable)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60B90, oracle length 74 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov ecx,[esi+14h] ; push esi ; mov dword ptr [esp+14h],0 ; call CConsoleVariableRegistry::Remove ; mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The derived vtable (0x012B9344) is stored, EH state 0 covers the body (the base is alive and has
// a non-trivial inline destructor whose only effect is restoring the base vtable 0x012B5868), the
// registry pointer at +0x14 removes `this` out of line (@ 0x00C5FF60). `this` is kept in the frame
// for the unwind funclet. The class comes from the manifest label; the registry type is descriptive.
struct CConsoleVariable;
struct CConsoleVariableRegistry {
    void Remove(CConsoleVariable* variable);   // 0x00C5FF60
};

struct CConsoleVariableBase {                // vtable 0x012B5868
    virtual void v00();
    ~CConsoleVariableBase() { }
};

struct CConsoleVariable : CConsoleVariableBase {   // vtable 0x012B9344
    unsigned char _pad[0x10];
    CConsoleVariableRegistry* m_pRegistry;   // +0x14
    ~CConsoleVariable();
};

CConsoleVariable::~CConsoleVariable()
{
    m_pRegistry->Remove(this);
}