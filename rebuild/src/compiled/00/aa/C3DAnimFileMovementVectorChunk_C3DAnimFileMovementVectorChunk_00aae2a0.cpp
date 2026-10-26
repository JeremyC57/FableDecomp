// C3DAnimFileMovementVectorChunk::C3DAnimFileMovementVectorChunk() -- base CBase_00aa8360(19523492) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAE2A0, oracle length 23 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x129e7a4 ; mov esi,ecx ; call CBase_00aa8360::CBase_00aa8360 ; mov dword ptr [esi],offset C3DAnimFileMovementVectorChunk::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor that passes the constant 19523492 (0x129e7a4) to the base constructor @ 0x00AA8360
// (an identifier/type code of this class in the base's scheme) and installs this class's vtable
// (0x0129E83C, named through rtti-vtables.tsv). One of 11 bodies of this shape.
struct CBase_00aa8360 {
    virtual void v00();
    CBase_00aa8360(long typeCode);              // 0x00AA8360
};

class C3DAnimFileMovementVectorChunk : public CBase_00aa8360 { public:
    virtual void v00();
    C3DAnimFileMovementVectorChunk();
};

C3DAnimFileMovementVectorChunk::C3DAnimFileMovementVectorChunk()
    : CBase_00aa8360(19523492)
{
}