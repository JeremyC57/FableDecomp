// CVtable_012b8ed4::Initialize(const void* device, void* window, bool multithreaded) -- vtable slot 1 of the OpenAL streaming buffer: load the OpenAL library through the global audio context when it is not loaded yet; true when the context has a module
// Leaf: ?Initialize@CVtable_012b8ed4@@UAE_NPBXPAX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57460, 37 bytes (+ int3 padding), thiscall `ret 0Ch` (the three arguments
//   are unused); slot 1 of 0x012B8ED4 and of the derived 0x012B8E20.
// Callee: CAudioContext::Initialize 0x00C565F0 on the global context 0x0143BDB0 (/GS, loads OpenAL32.dll; catalog
//   "CEngineWaterRenderer_InitializeOpenAL" is propagated, descriptive name), argument false.
// Name: descriptive (no PDB/RTTI); the parameters follow the DirectSound slot 1 0x00C56F70 (device GUID to
//   IDirectSound::Initialize, window to SetCooperativeLevel, the bool selecting the CoInitializeEx model).
// Shape: `xor eax,eax ; test ; setne` is a bool result of a pointer test; the module is re-read after the call.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);

// Retail OpenAL wrapper (no RTTI; the global instance at 0x0143BDB0): the landed CAudioContext model (0x00C575A0
// GetSourcei, 0x00C72AF0): module +4, lazily resolved entry points (+0x68 alGetSourcei), alGetProcAddress flag +0x104.
struct CAudioContext {
    void*            __vftable;                     // +0x00
    void*            Module;                        // +0x04 (0x0143BDB4)
    unsigned char    _pad08[0x60];
    PFN_alGetSourcei pfnGetSourcei;                 // +0x68 (0x0143BE18)
    unsigned char    _pad6c[0x98];
    bool             UseALGetProcAddress;           // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);         // 0x00C4D740
    bool Initialize(bool reload);                   // 0x00C565F0 (loads the OpenAL DLL; descriptive name)
    void GetSourcei(unsigned long source, long param, long* value)   // inline here (out of line at 0x00C575A0)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei")
                                                        : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

// The OpenAL streaming buffer of the libpk audio driver (vtable 0x012B8ED4, no RTTI; ctor 0x00C57780 landed as
// CVtable_012b8ed4, dtor 0x00C577B0): the OpenAL counterpart of the DirectSound CDSoundStreamBuffer (vtable 0x012B8D98),
// slot for slot -- 1 Initialize (DirectSound 0x00C56F70), 3 CreateAndPlay (0x00C57380), 4 Create (0x00C56D70), 6
// IsChunkReady (0x00C57070). The derived table 0x012B8E20 (ctor 0x00C573E0) overrides slots 0 and 4.
class CVtable_012b8ed4 {
public:
    virtual void v00();
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void v02();
    virtual unsigned char CreateAndPlay(unsigned long a, unsigned long b);           // slot 3: 0x00C57800
    virtual bool Create(unsigned long a);                                            // slot 4: 0x00C572C0 (false)
    virtual void v05();
    virtual bool IsChunkReady();                                                     // slot 6: 0x00C57AF0
    unsigned char  _pad04[0x600];
    unsigned long  m_604;                           // +0x604
    unsigned long  m_source;                        // +0x608 OpenAL source name
    unsigned char  _pad60c[8];
    bool           m_614;                           // +0x614
    bool           m_615;                           // +0x615
    unsigned char  _pad616[2];
    unsigned long  m_bufferCount;                   // +0x618
    unsigned long  m_queuedCount;                   // +0x61c
};

bool CVtable_012b8ed4::Initialize(const void* device, void* window, bool multithreaded)
{
    if (!g_AudioContext.Module)
        g_AudioContext.Initialize(false);
    return g_AudioContext.Module != 0;
}