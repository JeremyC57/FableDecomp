#pragma optimize("s",on)
// Create_CEnginePrimitiveGroupManager(void*) -- factory: new CEnginePrimitiveGroupManager(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D82E2, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x40 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CEnginePrimitiveGroupManager) = 0x40 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D3B9F on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CEnginePrimitiveGroupManager {
    unsigned char _body[0x40];       // sizeof(CEnginePrimitiveGroupManager) = 0x40
    CEnginePrimitiveGroupManager(void* owner);                   // 0x004D3B9F
};

CEnginePrimitiveGroupManager* __fastcall Create_CEnginePrimitiveGroupManager(void* owner)
{
    return new CEnginePrimitiveGroupManager(owner);
}