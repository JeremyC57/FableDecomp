// CTCDiggingSpot::OnSerialise(CPersistContext& context, bool fullState) -- persist "Enabled" (default true, full transfers only) and "Hidden" (default false)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2530, length 73 bytes.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0 (x2; manifest label
// Transfer<signed char>, a byte-identical instance). Literals "Enabled" @ 0x0126D58C, "Hidden" @ 0x012701EC.
// CTCDiggingSpot vtable 0x0127017C slot 1 (rtti-vtables.tsv; chunk label "CTCDiggingSpot::vfunc_1");
// slot 1 of the CTCBase-derived vtables is OnSerialise (33 classes).
// PDB CTCDiggingSpot (0x10 bytes): Enabled +0x0c, Hidden +0x0d.
// Shape: fullState is tested first and its slot is dead afterwards, so both one-byte default
// temporaries are homed in it (`lea eax,[esp+10h]` / `mov byte ptr [esp+1Ch],1|0`); `this` is
// advanced in place (`add esi,0Dh`) for the last member.
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCDiggingSpot {
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    bool          Enabled;                                  // +0x0c
    bool          Hidden;                                   // +0x0d
    void OnSerialise(CPersistContext& context, bool fullState);
};

void CTCDiggingSpot::OnSerialise(CPersistContext& context, bool fullState)
{
    if (fullState)
        context.Transfer("Enabled", Enabled, true);
    context.Transfer("Hidden", Hidden, false);
}
