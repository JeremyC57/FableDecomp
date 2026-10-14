// C3DPrimitive2::UploadBuffers() -- dispatch to the animated, repeated or static upload path
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A93380, oracle length 45 bytes.
// Callees: C3DPrimitive2::UploadAnimatedBuffers(ulong) @ 0x00A92870, C3DPrimitive2::UploadRepeatedBuffers(ulong, ulong) @ 0x00A932C0,
//          C3DPrimitive2::UploadStaticBuffers(ulong) @ 0x00A93320 (manifest labels both 0x00A932C0 and 0x00A93320
//          "UploadRepeatedBuffers"; the one-argument body is the plain static upload, see those files).
//
// Retail bytes:  mov eax,[ecx+48h] ; test eax,eax ; jbe S ; mov eax,[ecx+68h] ; push eax ; call UploadAnimatedBuffers ; ret ;
//                S: mov eax,[ecx+8] ; test eax,eax ; je T ; mov edx,[ecx+68h] ; push eax ; push edx ; call UploadRepeatedBuffers ; ret ;
//                T: mov eax,[ecx+68h] ; push eax ; call UploadStaticBuffers ; ret
// Retail layout (PDB struct_layouts_egor.tsv, 4 bytes lower after the 12-byte retail CArray at +0x20):
// +8 RepeatingMeshReps (long), +0x48 AnimatedBlockCount (ulong: the `jbe` is the unsigned `> 0` test),
// +0x68 InitFlags. Each branch returns the callee's bool straight through (`call ; ret`, the value-returning
// forwarder shape that VC7.1 does not turn into a sibling jump).
struct C3DPrimitive2 {
    virtual void v00();
    long          MaterialIndex;           // +0x04
    long          RepeatingMeshReps;       // +0x08
    unsigned char _pad_0x0c[0x48 - 0x0c];  // +0x0C .. +0x48
    unsigned long AnimatedBlockCount;      // +0x48
    unsigned char _pad_0x4c[0x68 - 0x4c];  // +0x4C .. +0x68
    unsigned long InitFlags;               // +0x68

    bool UploadBuffers();
    bool UploadAnimatedBuffers(unsigned long flags);                       // 0x00A92870
    bool UploadRepeatedBuffers(unsigned long flags, unsigned long reps);   // 0x00A932C0
    bool UploadStaticBuffers(unsigned long flags);                         // 0x00A93320
};

bool C3DPrimitive2::UploadBuffers()
{
    if (AnimatedBlockCount > 0)
        return UploadAnimatedBuffers(InitFlags);
    if (RepeatingMeshReps != 0)
        return UploadRepeatedBuffers(InitFlags, RepeatingMeshReps);
    return UploadStaticBuffers(InitFlags);
}
