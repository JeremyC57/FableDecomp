#pragma optimize("s",on)
// CTCCameraPointDead::CTCCameraPointDead(void*) -- base CEnginePrimitiveGroupManager(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3B9F, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager ; mov dword ptr [esi],offset CTCCameraPointDead::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004D3A03; the compiler then stores this class's vtable (0x0123B23C, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.

namespace N_004d3a03 { struct CEnginePrimitiveGroupManager {
    virtual void v00();
    CEnginePrimitiveGroupManager(void* argument);            // 0x004D3A03
}; } using N_004d3a03::CEnginePrimitiveGroupManager;

struct CTCCameraPointDead : CEnginePrimitiveGroupManager {
    virtual void v00();
    CTCCameraPointDead(void* argument);
};

CTCCameraPointDead::CTCCameraPointDead(void* argument)
    : CEnginePrimitiveGroupManager(argument)
{
}