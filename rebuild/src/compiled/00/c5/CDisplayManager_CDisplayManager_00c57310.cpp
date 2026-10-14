// CDisplayManager::~CDisplayManager() -- EH-framed destructor: own vtable (0x012B8DF8), its own virtual slot 5 through the constant vtable slot, the COM device at +8 released, a no-argument import called and the pointer cleared when present, then the inline base destructor restores the base vtable (0x012B5868)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57310, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],0 ; call dword ptr [vtable+14h] ; mov eax,[esi+8] ; test eax,eax ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; call [__imp_Shutdown] ; mov dword ptr [esi+8],0 ; skip: mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The virtual call stays virtual through an inline helper and resolves to the constant vtable
// slot; the device is a COM-style interface (`__stdcall` slot 2 = Release with `this` pushed). EH
// state 0 covers the body. The manifest label (`DestroyDevice`) names the inlined helper; the
// class is the one whose vtable is stored.
extern "C" __declspec(dllimport) void __stdcall ShutdownDisplay_00c57310();   // IAT 0x01440648
struct IUnknown_00c57310 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct CDisplayManagerBase_00c57310 {      // vtable 0x012B5868
    virtual void v00();
    ~CDisplayManagerBase_00c57310() { }
};

struct CDisplayManager : CDisplayManagerBase_00c57310 {   // vtable 0x012B8DF8
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void ReleaseResources();       // slot 5
    __forceinline void DoReleaseResources() { ReleaseResources(); }
    unsigned char _pad[4];
    IUnknown_00c57310* m_pDevice;          // +8
    ~CDisplayManager();
};

CDisplayManager::~CDisplayManager()
{
    DoReleaseResources();
    if (m_pDevice) {
        m_pDevice->Release();
        ShutdownDisplay_00c57310();
        m_pDevice = 0;
    }
}