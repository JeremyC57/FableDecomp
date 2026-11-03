// CAudioContextEAX::~CAudioContextEAX() -- EH-framed destructor: own vtable (0x012BEDF4), the EAX property setter resolved on first use into the global @ 0x0143BEA0 (through CAudioContext::GetProcAddress @ 0x00C4D740 on g_AudioContext @ 0x0143BDB0), called with the property GUID @ 0x012BEE90, property 5, source 0, a -10000 (muted) value and size 4, the enabled byte at +4 cleared, then the out-of-line base destructor (@ 0x00C6AC50)
// Leaf: ??1CAudioContextEAX@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71CF0, oracle length 138 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov eax,[g_pfnEAXSet] ; test eax,eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esp+4],-10000 ; jne have ; push offset "EAXSet" ; mov ecx,offset g_resolver ; call CResolver::Resolve ; mov [g_pfnEAXSet],eax ; have: push 4 ; lea ecx,[esp+8] ; push ecx ; push 0 ; push 5 ; push offset guid ; call eax ; add esp,14h ; mov ecx,esi ; mov byte ptr [esi+4],0 ; mov dword ptr [esp+14h],-1 ; call ~CAudioContextBase ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// EH state 0 belongs to a member with an inline empty destructor (no code, only the state);
// the function pointer is called through eax straight after the resolve. The manifest label
// (`Initialize`) is propagated for a destructor that mutes the listener before tearing down.
typedef long (__cdecl* PFNEAXSET_00c71cf0)(const void* guid, unsigned long property, unsigned long source, void* value, unsigned long size);
extern PFNEAXSET_00c71cf0 g_pfnEAXSet_0143bea0;    // 0x0143BEA0
struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
// 0x012BEE90 (owner decision 2026-11-03 D3): the original GameCODA 2.10 SAL object CSalOpenALReverb::Update relocates the C++
//   ?SAL_EAX30_ListenerProperties@@3U_GUID@@B here (byte-identical 1115-byte body, Codex pass 12 SDK_COMPARISON 00c71870).
extern const _GUID SAL_EAX30_ListenerProperties;   // 0x012BEE90 {A8FA6882-B476-11D3-BDB9-00C0F02DDF87}

// Naming wave 17: the audio context object @ 0x0143BDB0 is the landed descriptive g_AudioContext (struct CAudioContext), unified across
//   the libpk rows (c675); was CProcResolver_00c71cf0 g_procResolver_0143bdb0 with Resolve (name-only).
struct CAudioContext {
    void* GetProcAddress(const char* name);        // 0x00C4D740
};
extern CAudioContext g_AudioContext;               // 0x0143BDB0

struct CAudioContextBase_00c71cf0 {
    virtual void v00();
    ~CAudioContextBase_00c71cf0();         // 0x00C6AC50
};

struct CContextMarker_00c71cf0 {           // a member whose destructor is empty but not trivial
    unsigned char _body[4];
    ~CContextMarker_00c71cf0() { }
};

struct CAudioContextEAX : CAudioContextBase_00c71cf0 {   // vtable 0x012BEDF4
    bool m_enabled;                        // +4
    unsigned char _pad[3];
    CContextMarker_00c71cf0 m_marker;      // +8
    ~CAudioContextEAX();
};

CAudioContextEAX::~CAudioContextEAX()
{
    long volume = -10000;
    if (!g_pfnEAXSet_0143bea0)
        g_pfnEAXSet_0143bea0 = (PFNEAXSET_00c71cf0)g_AudioContext.GetProcAddress("EAXSet");
    g_pfnEAXSet_0143bea0(&SAL_EAX30_ListenerProperties, 5, 0, &volume, 4);
    m_enabled = false;
}