// CAudioContextEAX::~CAudioContextEAX() -- EH-framed destructor: own vtable (0x012BEDF4), the EAX property setter resolved on first use into the global @ 0x0143BEA0 (through the resolver object @ 0x0143BDB0, @ 0x00C4D740), called with the property GUID @ 0x012BEE90, property 5, source 0, a -10000 (muted) value and size 4, the enabled byte at +4 cleared, then the out-of-line base destructor (@ 0x00C6AC50)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71CF0, oracle length 138 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov eax,[g_pfnEAXSet] ; test eax,eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esp+4],-10000 ; jne have ; push offset "EAXSet" ; mov ecx,offset g_resolver ; call CResolver::Resolve ; mov [g_pfnEAXSet],eax ; have: push 4 ; lea ecx,[esp+8] ; push ecx ; push 0 ; push 5 ; push offset guid ; call eax ; add esp,14h ; mov ecx,esi ; mov byte ptr [esi+4],0 ; mov dword ptr [esp+14h],-1 ; call ~CAudioContextBase ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// EH state 0 belongs to a member with an inline empty destructor (no code, only the state);
// the function pointer is called through eax straight after the resolve. The manifest label
// (`Initialize`) is propagated for a destructor that mutes the listener before tearing down.
typedef long (__cdecl* PFNEAXSET_00c71cf0)(const void* guid, unsigned long property, unsigned long source, void* value, unsigned long size);
extern PFNEAXSET_00c71cf0 g_pfnEAXSet_0143bea0;    // 0x0143BEA0
extern const unsigned char g_eaxListenerGuid[16];  // 0x012BEE90

struct CProcResolver_00c71cf0 {
    PFNEAXSET_00c71cf0 Resolve(const char* name);   // 0x00C4D740
};
extern CProcResolver_00c71cf0 g_procResolver_0143bdb0;   // 0x0143BDB0

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
        g_pfnEAXSet_0143bea0 = g_procResolver_0143bdb0.Resolve("EAXSet");
    g_pfnEAXSet_0143bea0(g_eaxListenerGuid, 5, 0, &volume, 4);
    m_enabled = false;
}
