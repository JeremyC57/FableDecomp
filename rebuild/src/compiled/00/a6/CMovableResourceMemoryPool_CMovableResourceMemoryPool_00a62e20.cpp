// CMovableResourceMemoryPool::CMovableResourceMemoryPool() -- base CEnginePrimitiveTypeInfo() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62E20, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo ; mov dword ptr [esi],offset CMovableResourceMemoryPool::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00A02D20 runs, then the compiler stores this class's vtable (0x0129D6A0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CEnginePrimitiveTypeInfo {
    virtual void v00();
    CEnginePrimitiveTypeInfo();                           // 0x00A02D20
};

struct CMovableResourceMemoryPool : CEnginePrimitiveTypeInfo {
    virtual void v00();                    // this class has its own vtable
    CMovableResourceMemoryPool();
};

CMovableResourceMemoryPool::CMovableResourceMemoryPool()
    : CEnginePrimitiveTypeInfo()
{
}