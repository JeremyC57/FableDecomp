// CActionParamCountedPointer_00c577b0::~CActionParamCountedPointer_00c577b0() -- EH-framed destructor: stores the vtable (0x012B8ED4), calls its own virtual Release (slot 5, devirtualised through the known vtable slot @ 0x012B8EE8), then the inline base destructor restores the base vtable (0x012B5868)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C577B0, oracle length 71 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],0 ; call dword ptr [vtable+0x14] ; mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// A virtual call on `this` inside a destructor is devirtualised by VC7.1 to a direct call; through
// an inline non-virtual helper the call stays virtual and, the dynamic type being known, becomes a
// call through the class's own vtable slot at a constant address (`call dword ptr [vtable+14h]`). EH state 0 covers the body (the base is alive; its inline empty destructor
// only restores the base vtable). The manifest label (`~CActionParamCCountedPointer<...>`) names
// the shape; the class is named by address. One of 2 bodies of this shape.
struct CActionParamBase_00c577b0 {             // vtable 0x012B5868
    virtual void v00();
    ~CActionParamBase_00c577b0() { }
};

struct CActionParamCountedPointer_00c577b0 : CActionParamBase_00c577b0 {    // vtable 0x012B8ED4
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void Release();                // slot 5
    __forceinline void DoRelease() { Release(); }   // keeps the call virtual: written directly in the destructor it would be a direct call
    ~CActionParamCountedPointer_00c577b0();
};

CActionParamCountedPointer_00c577b0::~CActionParamCountedPointer_00c577b0()
{
    DoRelease();
}