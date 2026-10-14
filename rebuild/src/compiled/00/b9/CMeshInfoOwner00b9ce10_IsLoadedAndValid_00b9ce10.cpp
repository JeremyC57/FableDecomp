// CMeshInfoOwner_00b9ce10::IsLoadedAndValid() -- long-valued "both checks pass" predicate on `this`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9CE10, length 34 bytes.
// Callees (both on `this`, no arguments, bool results): C3DMeshInfo::IsLoaded @ 0x00A23E40 (walks the mesh
// list at +0xc) and the trivial `mov al,1` query @ 0x00A23C60 (manifest "CThingFilter_IsUnconscious::
// operator()" is a propagated label for a constant-true body). The manifest module label
// "CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction" is propagated from an unrelated AI body; the
// class is named descriptively after what the callees look at.
// Shape: `if (a() && b()) return 1; return 0;` with a LONG result (`mov eax,1` / `xor eax,eax`, not `al`).
struct CMeshInfoOwner_00b9ce10 {
    bool IsLoaded();                        // 0x00A23E40
    bool IsValid();                         // 0x00A23C60
    long IsLoadedAndValid();
};

long CMeshInfoOwner_00b9ce10::IsLoadedAndValid()
{
    if (IsLoaded() && IsValid())
        return 1;
    return 0;
}