// CTCCameraPointTrack::OnSerialseLeaf(CPersistContext& context) -- saves/loads the 2 members StringLength / CageRadius by name (defaults 7.0f / 2.0f)
// Leaf: ?OnSerialseLeaf@CTCCameraPointTrack@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D0F0, 73 bytes (+ int3 padding to 80), thiscall, `ret 4`.
// Identity (proven): CTCCameraPointTrack::OnSerialseLeaf(CPersistContext&); evidence: it is slot 33 of vtable 0x0123B0E4, whose RTTI is .?AVCTCCameraPointTrack@@ (stored by 0x004D3AF4), the slot the landed CTCCameraPointGeneralCase::OnSerialseLeaf 0x0075D1C0 fills; the literals StringLength / CageRadius transfer the Ego_r.pdb members CameraStringLength (+0x40) / CameraCageRadius (+0x44), as in that sibling; the literals StringLength / CageRadius name exactly the Ego_r.pdb members of CTCCameraPointTrack (the only Ego_r class with those members; the track's members sit at their Ego_r offsets, offsets asserted), and the Ego_r/FableWin.pdb name sets hold ?OnSerialseLeaf@CTCCameraPointTrack@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "CTCCameraPointTrack::vfunc_33" is propagated / a placeholder (family J014: six two-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<float> 0x00410620, Transfer<float> 0x00410620.
// Shape: context in edi, this in esi; each default is a temporary in a dead stack slot (the first in the `push ecx` slot,
//   the second in the context argument slot); the second member's address is `add esi, N` in place. Model: agent952 H036
//   (the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 shape).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CTCCameraPointTrack {                         // RTTI .?AVCTCCameraPointTrack@@, vtable 0x0123B0E4
public:
    virtual void OnSerialseLeaf(CPersistContext& context);   // slot 33 (sic, PDB spelling)
    unsigned char _base04[0x40 - 4];               // CTCCameraPointDefinitionBase
    float          CameraStringLength;                // +0x40 (Ego_r +0x40)
    float          CameraCageRadius;                  // +0x44 (Ego_r +0x44)
};

void CTCCameraPointTrack::OnSerialseLeaf(CPersistContext& context)
{
    context.Transfer("StringLength", CameraStringLength, 7.0f);
    context.Transfer("CageRadius", CameraCageRadius, 2.0f);
}
