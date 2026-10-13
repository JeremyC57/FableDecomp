// CEnginePrimitiveManagerMeshBase::CEnginePrimitiveManagerMeshBase(void*) -- base CEnginePrimitiveManagerOcclusionBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBC30, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase ; mov dword ptr [esi],offset CEnginePrimitiveManagerMeshBase::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00B90270; the compiler then stores this class's vtable (0x012A815C, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.

struct CEnginePrimitiveManagerOcclusionBase {
    virtual void v00();
    CEnginePrimitiveManagerOcclusionBase(void* argument);            // 0x00B90270
};

struct CEnginePrimitiveManagerMeshBase : CEnginePrimitiveManagerOcclusionBase {
    virtual void v00();
    CEnginePrimitiveManagerMeshBase(void* argument);
};

CEnginePrimitiveManagerMeshBase::CEnginePrimitiveManagerMeshBase(void* argument)
    : CEnginePrimitiveManagerOcclusionBase(argument)
{
}
