// std::_Dest_val<std::allocator<CMacro>, CMacro>(CMacro*) -- destroy one element in place: three owned members, then the base
// Leaf: allocator<CMacro>,CMacro>
// Retail: Fable.exe (TLC, VC7.1) @ 0x004047A0, 35 bytes. Callees: member destructor A (+0x14), member destructor B 0x0059A340 (+0x10, +0x0C), base destructor 0x00595B40 (tail).
//
// STLport's allocator "destroy" hook: a vector shrinking or dying calls this on each element, and it is
// nothing more than the element's destructor inlined. The element owns three sub-objects at +0x0C,
// +0x10 and +0x14 which go in reverse declaration order, and then the base-class destructor finishes
// the job. The CMacro name on this address comes from the symbol tables; the PDB's CMacro (Name, Code,
// ParamDef, DebugContextString) does not put destructible members at these offsets, so this body
// appears to serve some other 0x18-plus-byte element type as well.
//
// Matching: a straight run of __fastcall member-destructor calls; the base destructor on `self` is the
// last call and becomes the `jmp` tail.
struct MemberA { char pad[4]; };    // sub-object type at +0x14
struct MemberB { char pad[4]; };    // sub-object type at +0x0C and +0x10
struct Base { char pad[4]; };       // the element's base class

extern "C" void __fastcall dtorA(MemberA* p);   // destructor of the +0x14 member
extern "C" void __fastcall dtorB(MemberB* p);   // destructor of the +0x10 / +0x0C members (0x0059A340)
extern "C" void __fastcall dtorBase(Base* p);   // base-class destructor (0x00595B40, tail call)

struct Host {
    char pad0[0xc];   // base-class data
    MemberB m_c;      // owned sub-object
    MemberB m_10;     // owned sub-object
    MemberA m_14;     // last declared, destroyed first
};

void __fastcall Dest_val(Host* self)
{
    // Reverse declaration order, then the base.
    dtorA(&self->m_14);
    dtorB(&self->m_10);
    dtorB(&self->m_c);
    dtorBase((Base*)self);
}
