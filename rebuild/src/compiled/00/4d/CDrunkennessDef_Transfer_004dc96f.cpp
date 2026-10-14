#pragma optimize("y", off)
#pragma optimize("s", on)
// CDrunkennessDef::Transfer(CPersistContext&) -- persist the drunkenness threshold multiplier with a zero default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC96F, length 34 bytes.
// Callees: CPersistContext::Transfer<float>(const char*, float&, const float&) @ 0x00410620 (the name literal at 0x01240CA8, the member at +0x28, the default built
// in the 4-byte frame local through `fldz`/`fstp`). PDB CDrunkennessDef (44 bytes): DrunkennessThresholdMult at +0x28 (no retail shift: defs hold no
// containers before it). Frame-pointer size TU of the 0x004Dxxxx definition module (`push ebp ; mov ebp,esp ; push ecx`, `leave`). The method is
// the virtual persist hook (mangled `?Transfer@CDrunkennessDef@@UAEXAAVCPersistContext@@@Z`).
// Leaf: Transfer
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
};

struct CDrunkennessDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad[0x24];                      // +0x04 .. +0x28: the definition base's members
    float         DrunkennessThresholdMult;        // +0x28
};

void CDrunkennessDef::Transfer(CPersistContext& context)
{
    context.Transfer("DrunkennessThresholdMult", DrunkennessThresholdMult, 0.0f);
}