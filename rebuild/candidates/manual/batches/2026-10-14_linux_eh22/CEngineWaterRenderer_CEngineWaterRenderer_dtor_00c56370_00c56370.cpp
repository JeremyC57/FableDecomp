// CEngineWaterRenderer::~CEngineWaterRenderer() -- EH-framed destructor (vtable 0x012B8C18, base CBase 0x012B5868): when the context at +0x100 is live the unbind function pointer at +0x10 is called with 0, the destroy function pointer at +0x18 resolved on demand (`GetProcAddress(m_hModule, "alcDestroyContext")`) and called with the context, which is nulled; the water generator destroyed (@ 0x00C56290), the module at +4 freed, then the mixer handle member at +0x110 closed (@ 0x00C716C0, state 0) and the inlined CBase destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56370, oracle length 177 bytes. Built under /EHsc.
// Retail bytes:  ... mov dword ptr [esi],012B8C18h ; mov eax,[esi+100h] ; test eax,eax ; mov dword ptr [esp+10h],1 ; je gen ; push 0 ; call [esi+10h] ; mov eax,[esi+18h] ; add esp,4 ; test eax,eax ; jne have ; mov eax,[esi+4] ; push offset "alcDestroyContext" ; push eax ; call [__imp_GetProcAddress] ; mov [esi+18h],eax ; have: mov ecx,[esi+100h] ; push ecx ; call [esi+18h] ; add esp,4 ; mov dword ptr [esi+100h],0 ; gen: mov ecx,esi ; call DestroyWaterGenerator ; mov eax,[esi+4] ; test eax,eax ; je close ; push eax ; call [__imp_FreeLibrary] ; mov dword ptr [esi+4],0 ; close: lea ecx,[esi+110h] ; mov byte ptr [esp+10h],0 ; call CMixerHandle::Close ; mov ecx,[esp+8] ; mov dword ptr [esi],012B5868h ; ...
// The state 1 -> 0 store is a byte store in retail. Named from the manifest (the generator
// callee carries the class); the OpenAL function pointers are cdecl.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
extern "C" __declspec(dllimport) long __stdcall FreeLibrary(void* module);                         // IAT 0x0143FE3C

typedef void (__cdecl* PFN_alcMakeContextCurrent_00c56370)(void* context);
typedef void (__cdecl* PFN_alcDestroyContext_00c56370)(void* context);

struct CBase_00c56370 {                    // vtable 0x012B5868
    virtual void v00();
    ~CBase_00c56370() { }
};

struct CMixerHandle_00c56370 {             // +0x110
    unsigned char _body[8];
    ~CMixerHandle_00c56370();              // 0x00C716C0
};

struct CEngineWaterRenderer_00c56370 : CBase_00c56370 {   // vtable 0x012B8C18
    void* m_hModule;                       // +4
    long m_8;
    long m_c;
    PFN_alcMakeContextCurrent_00c56370 m_pfnMakeCurrent;   // +0x10
    long m_14;
    PFN_alcDestroyContext_00c56370 m_pfnDestroyContext;    // +0x18
    unsigned char _pad[0xe4];
    void* m_pContext;                      // +0x100
    unsigned char _pad2[0xc];
    CMixerHandle_00c56370 m_mixer;         // +0x110
    void DestroyWaterGenerator();          // 0x00C56290
    ~CEngineWaterRenderer_00c56370();
};

CEngineWaterRenderer_00c56370::~CEngineWaterRenderer_00c56370()
{
    if (m_pContext) {
        m_pfnMakeCurrent(0);
        if (!m_pfnDestroyContext)
            m_pfnDestroyContext = (PFN_alcDestroyContext_00c56370)GetProcAddress(m_hModule, "alcDestroyContext");
        m_pfnDestroyContext(m_pContext);
        m_pContext = 0;
    }
    DestroyWaterGenerator();
    if (m_hModule) {
        FreeLibrary(m_hModule);
        m_hModule = 0;
    }
}
