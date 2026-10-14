// CCreatureAction_SummonerFlameSlice::~CCreatureAction_SummonerFlameSlice() -- EH-framed destructor: own vtable (0x012B8FCC), its own virtual slot 5 through the constant vtable slot, the polymorphic member at +0x1c destroyed inline (its vtable 0x012B8FC8 stored, its field at +0x20 passed to an import), then the inline base destructor (0x012B5868)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D790, oracle length 88 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],1 ; call dword ptr [vtable+14h] ; lea eax,[esi+20h] ; push eax ; mov dword ptr [esi+1Ch],offset member vtable ; call [__imp_DeleteCriticalSection] ; mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 1 covers the body (two tracked objects remain: the member and the base). The virtual
// call stays virtual through an inline helper (written directly it would be devirtualised to a
// direct call) and resolves to the constant vtable slot. The member's inline destructor stores
// its vtable and passes its critical section to the `__stdcall` import. The class comes from the
// manifest label; the rest is descriptive.
struct CRITICAL_SECTION_00c5d790;
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION_00c5d790*);   // IAT 0x0143FDE4

struct CActionBase_00c5d790 {              // vtable 0x012B5868
    virtual void v00();
    ~CActionBase_00c5d790() { }
};

struct CSectionHolder_00c5d790 {           // +0x1c: vtable 0x012B8FC8
    virtual void v00();
    unsigned char m_section[4];            // +0x20
    ~CSectionHolder_00c5d790() { DeleteCriticalSection((CRITICAL_SECTION_00c5d790*)m_section); }
};

struct CCreatureAction_SummonerFlameSlice : CActionBase_00c5d790 {   // vtable 0x012B8FCC
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Release();                // slot 5
    __forceinline void DoRelease() { Release(); }
    unsigned char _pad[0x18];
    CSectionHolder_00c5d790 m_holder;      // +0x1c
    ~CCreatureAction_SummonerFlameSlice();
};

CCreatureAction_SummonerFlameSlice::~CCreatureAction_SummonerFlameSlice()
{
    DoRelease();
}