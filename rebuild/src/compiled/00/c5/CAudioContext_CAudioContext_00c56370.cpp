// CAudioContext::~CAudioContext() -- EH-framed destructor of libpk's OpenAL wrapper (vtable 0x012B8C18, base vtable 0x012B5868): destroy a live context (inlined DestroyContext: alcMakeContextCurrent(0), alcDestroyContext resolved on demand from the DLL), CloseDevice 0x00C56290, free the OpenAL DLL module (+0x04), then destroy the +0x110 member (0x00C716C0, EH state 0) and store the base vtable
// Leaf: ??1CAudioContext@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56370, oracle length 177 bytes.
// Retail bytes:  ... mov dword ptr [esi],012B8C18h ; mov eax,[esi+100h] ; test eax,eax ; mov dword ptr [esp+10h],1 ; je gen ; push 0 ; call [esi+10h] ; mov eax,[esi+18h] ; add esp,4 ; test eax,eax ; jne have ; mov eax,[esi+4] ; push offset "alcDestroyContext" ; push eax ; call [__imp_GetProcAddress] ; mov [esi+18h],eax ; have: mov ecx,[esi+100h] ; push ecx ; call [esi+18h] ; add esp,4 ; mov dword ptr [esi+100h],0 ; gen: mov ecx,esi ; call DestroyWaterGenerator ; mov eax,[esi+4] ; test eax,eax ; je close ; push eax ; call [__imp_FreeLibrary] ; mov dword ptr [esi+4],0 ; close: lea ecx,[esi+110h] ; mov byte ptr [esp+10h],0 ; call CMixerHandle::Close ; mov ecx,[esp+8] ; mov dword ptr [esi],012B5868h ; ...
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC56370 CEngineWaterRenderer dtor = CAudioContext"): the callee the landed
//   row called CEngineWaterRenderer::DestroyWaterGenerator is the landed CAudioContext::CloseDevice 0x00C56290, the context
//   block is agent440's CAudioContext::DestroyContext 0x00C559C0 inlined (agent440 notes the destructor inlines it), and the
//   offsets are agent440's CAudioContext layout (Module +0x04, alcMakeContextCurrent +0x10, alcDestroyContext +0x18, Context
//   +0x100) whose vtable 0x012B8C18 this destructor stores (global instance 0x0143BDB0). Descriptive names (no PDB covers libpk).
// Label correction: the landed row was CEngineWaterRenderer::~CEngineWaterRenderer (catalog _global::CEngineWaterRenderer_Destructor,
//   a forge auto-name); the same callees, typed as the audio context's.
// Virtual: slot 0 of 0x012B8C18 is the scalar deleting destructor 0x00C565D0 (this body, then CPKObject::operator delete
//   0x00C3C370), so the destructor is virtual through the libpk root CPKObject (vtable 0x012B5868), whose inline
//   destructor's store of 0x012B5868 ends the body.
// Shape: the state 1 -> 0 store is a byte store in retail; the OpenAL function pointers are cdecl.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
extern "C" __declspec(dllimport) long __stdcall FreeLibrary(void* module);                         // IAT 0x0143FE3C

typedef unsigned char (__cdecl* PFN_alcMakeContextCurrent)(void* context);
typedef void (__cdecl* PFN_alcDestroyContext)(void* context);

struct CPKObject {                         // vtable 0x012B5868: the libpk root (inline virtual destructor)
    virtual ~CPKObject() { }
};

struct CMixerHandle {                      // the +0x110 member (catalog Audio_MixerHandle_Close)
    unsigned char _body[8];
    ~CMixerHandle();                       // 0x00C716C0
};

struct CAudioContext : CPKObject {         // vtable 0x012B8C18
    void*                     Module;                    // +0x04
    void*                     pfnOpenDevice;             // +0x08
    void*                     pfnCreateContext;          // +0x0c
    PFN_alcMakeContextCurrent pfnMakeContextCurrent;     // +0x10
    void*                     pfnCloseDevice;            // +0x14
    PFN_alcDestroyContext     pfnDestroyContext;         // +0x18
    unsigned char             _pad1c[0xfc - 0x1c];
    void*                     Device;                    // +0xfc
    void*                     Context;                   // +0x100
    unsigned char             _pad104[0xc];
    CMixerHandle              m_mixer;                   // +0x110
    void CloseDevice();                                  // 0x00C56290
    ~CAudioContext();
};

CAudioContext::~CAudioContext()
{
    if (Context) {
        pfnMakeContextCurrent(0);
        if (!pfnDestroyContext)
            pfnDestroyContext = (PFN_alcDestroyContext)GetProcAddress(Module, "alcDestroyContext");
        pfnDestroyContext(Context);
        Context = 0;
    }
    CloseDevice();
    if (Module) {
        FreeLibrary(Module);
        Module = 0;
    }
}
