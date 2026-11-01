// CDefaultAllocator_00c166f0::CDefaultAllocator_00c166f0() -- root polymorphic class: only the vtable store
// Naming (naming wave 21): CVtable_012b2ab8 -> CDefaultAllocator_00c166f0, the libpk default malloc/free allocator object at 0x0143BB34 (dynamic initializer 0x01224B40; Allocate 0x00C16700 / Free 0x00C16720) the StoreSound*Allocator stores 0x00C16690 / 0x00C166D0 / 0x00C166B0 fall back to (descriptive, libpk).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C166F0, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0x12b2ab8 ; ret 
// Default constructor of a root polymorphic class with nothing to initialise: installs the vtable
// (0x012B2AB8, rtti-vtables.tsv) and returns `this`. One of the tiny-body shapes.
class CDefaultAllocator_00c166f0 { public:
    virtual void v00();
    CDefaultAllocator_00c166f0();
};

CDefaultAllocator_00c166f0::CDefaultAllocator_00c166f0()
{
}