// CTCDRegionEntrance::OnSerialise(CPersistContext& context, bool fullState) -- persist the "Active" flag (default true)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778EF0, length 33 bytes.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0 (manifest label
// Transfer<signed char>, a byte-identical instance). Literal "Active" @ 0x0125A25C.
// CTCDRegionEntrance vtable 0x0123BA74 slot 1 (rtti-vtables.tsv; chunk label "CTCDRegionEntrance::vfunc_1");
// slot 1 of the CTCBase-derived vtables is OnSerialise (33 classes); `ret 8` = two arguments, the
// second unused. PDB CTCDRegionEntrance (0x10 bytes): Active +0x0c.
// Shape: the `true` default is a one-byte temporary in the high byte of the `push ecx` frame slot,
// stored after the argument pushes; `this` is advanced in place (`add ecx,0Ch`) to form &Active.
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCDRegionEntrance {
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    bool          Active;                                   // +0x0c
    void OnSerialise(CPersistContext& context, bool fullState);
};

void CTCDRegionEntrance::OnSerialise(CPersistContext& context, bool fullState)
{
    context.Transfer("Active", Active, true);
}
