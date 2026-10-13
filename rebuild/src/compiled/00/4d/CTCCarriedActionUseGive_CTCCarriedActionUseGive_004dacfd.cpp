#pragma optimize("s",on)
// CTCCarriedActionUseGive::CTCCarriedActionUseGive(void*) -- base CEnginePrimitiveGroupManager(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DACFD, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager ; mov dword ptr [esi],offset CTCCarriedActionUseGive::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DAC61; the compiler then stores this class's vtable (0x0123FAB4, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.

struct CEnginePrimitiveGroupManager {
    virtual void v00();
    CEnginePrimitiveGroupManager(void* argument);            // 0x004DAC61
};

struct CTCCarriedActionUseGive : CEnginePrimitiveGroupManager {
    virtual void v00();
    CTCCarriedActionUseGive(void* argument);
};

CTCCarriedActionUseGive::CTCCarriedActionUseGive(void* argument)
    : CEnginePrimitiveGroupManager(argument)
{
}