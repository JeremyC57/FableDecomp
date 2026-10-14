// CEngineInternalPrimitiveParticleGroup::PrimTypeMatchesGroup(long type) -- 1 when the primitive type is one of the eight particle-group member types (0x18, 0x1c, 0x1a, 0x1b, 0x1d, 7, 0x14, 0x1e), else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBF00, length 57 bytes (64 with padding).
// Callees: none. Shape: a member whose `this` is unused (`ret 4`; a static would be `__cdecl` with a plain `ret`); the eight compares are in source order (an `||` chain, not a switch, which VC would sort), the result is
// a full register (`xor eax,eax` / `mov eax,1`), so the function returns `long`, not bool.
struct CEngineInternalPrimitiveParticleGroup {
    long PrimTypeMatchesGroup(long type);
};

long CEngineInternalPrimitiveParticleGroup::PrimTypeMatchesGroup(long type)
{
    if (type == 0x18 || type == 0x1c || type == 0x1a || type == 0x1b ||
        type == 0x1d || type == 7 || type == 0x14 || type == 0x1e)
        return 1;
    return 0;
}
