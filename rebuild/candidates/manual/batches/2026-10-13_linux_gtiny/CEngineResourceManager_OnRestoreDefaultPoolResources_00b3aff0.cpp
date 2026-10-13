// CEngineResourceManager::OnRestoreDefaultPoolResources() -- sibling jump to CGPUScratchBufferTextureManager::EndShutdown on the embedded member at +0x3190
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3AFF0, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 0x3190 ; jmp 0xf36d30
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CGPUScratchBufferTextureManager {
    long EndShutdown();         // 0x00F36D30
};

struct CEngineResourceManager {
    unsigned char _pad[0x3190];
    CGPUScratchBufferTextureManager m_member;
    long OnRestoreDefaultPoolResources();
};

long CEngineResourceManager::OnRestoreDefaultPoolResources()
{
    return m_member.EndShutdown();
}
