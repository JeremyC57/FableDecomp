// CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume(CEngineSubPrimitiveBase&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91340, oracle length 16 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov edx,[eax+0Ch] ; mov [esp+4],edx ; jmp RenderBoundingBox
// The sub-primitive overload just forwards to the parent-primitive overload
// (0x00B91140, manifest: RenderBoundingBox(CEngineInternalPrimitiveBase*)). VC7.1 turns the
// forward into a tail jump because both are __thiscall with one 4-byte stack argument: it
// overwrites the incoming argument slot with sub.Parent and jumps.
// Member evidence: Ego_r.pdb CEngineSubPrimitiveBase +0x0C = CEngineInternalPrimitiveBase* Parent.

struct CEngineInternalPrimitiveBase;

struct CEngineSubPrimitiveBase {
    void*                         __vftable;    // +0x00
    unsigned long                 RenderFlags;  // +0x04 (PDB)
    unsigned char                 Alpha;        // +0x08 (PDB)
    unsigned char                 _pad_0x09[3];
    CEngineInternalPrimitiveBase* Parent;       // +0x0C (PDB) owning internal primitive
};

struct CEnginePrimitiveManagerOcclusionBase {
    // 0x00B91140: draws the occlusion bounding box of one internal primitive (not in this TU).
    void RenderBoundingBox(CEngineInternalPrimitiveBase* primitive);
    // 0x00B91340: this TU.
    void RenderBoundingVolume(CEngineSubPrimitiveBase& subPrimitive);
};

void CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume(CEngineSubPrimitiveBase& subPrimitive)
{
    // A sub-primitive has no volume of its own; draw the parent's box.
    RenderBoundingBox(subPrimitive.Parent);
}
