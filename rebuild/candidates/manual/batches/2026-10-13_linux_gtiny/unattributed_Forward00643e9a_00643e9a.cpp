// CClass_00643e9a::Forward_00643e9a() -- sibling jump to CMiniMapDisplay::UpdateMarkers on the embedded member at +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643E9A, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 0x28 ; jmp 0x82b7c0
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CMiniMapDisplay {
    long UpdateMarkers();       // 0x0082B7C0
};

struct CClass_00643e9a {
    unsigned char _pad[0x28];
    CMiniMapDisplay m_member;
    long Forward_00643e9a();
};

long CClass_00643e9a::Forward_00643e9a()
{
    return m_member.UpdateMarkers();
}
