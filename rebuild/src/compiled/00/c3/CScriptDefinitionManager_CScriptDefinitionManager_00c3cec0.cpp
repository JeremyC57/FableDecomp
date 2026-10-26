// CScriptDefinitionManager::~CScriptDefinitionManager() -- EH-framed destructor: stores the vtable (0x012B5A84), shuts down out of line (@ 0x00C40800), then the base destructor (out of line @ 0x00C4CF40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CEC0, oracle length 79 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],0 ; call Shutdown ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The derived vtable is stored, EH state 0 covers the body (the base is alive and its destructor is
// out of line), the out-of-line Shutdown runs on `this`, then the base destructor (state -1). `this`
// is kept in the frame for the unwind funclet, which destroys the base. The class comes from the
// manifest label; the base and the helper are descriptive. One of 4 bodies of this shape.
namespace N_00c4cf40 { struct CScriptDefinitionManagerBase {
    virtual void v00();
    ~CScriptDefinitionManagerBase();                          // 0x00C4CF40
}; } using N_00c4cf40::CScriptDefinitionManagerBase;

class CScriptDefinitionManager : public CScriptDefinitionManagerBase { public:                 // vtable 0x012B5A84
    void Shutdown_00c40800();                       // 0x00C40800
    ~CScriptDefinitionManager();
};

CScriptDefinitionManager::~CScriptDefinitionManager()
{
    Shutdown_00c40800();
}