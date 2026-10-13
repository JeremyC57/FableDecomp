// CThingFilter_IsNot::operator()(CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661640, oracle length 12 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; cmp eax,[ecx] ; setne al ; ret 4
// PDB CThingFilter_IsNot: +0x00 CThing* Thing. The predicate is true for every thing except the one
// it was built with. The `?:` form gives the retail `cmp ; setne al` directly.
struct CThing;

struct CThingFilter_IsNot {
    CThing* Thing;                          // +0x00
    bool operator()(CThing* thing);
};

bool CThingFilter_IsNot::operator()(CThing* thing)
{
    return (thing != this->Thing) ? true : false;
}
