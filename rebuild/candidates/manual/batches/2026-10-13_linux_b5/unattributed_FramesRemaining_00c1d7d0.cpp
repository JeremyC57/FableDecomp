// <unattributed>::FramesRemaining() -- Member20 - Query()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D7D0, oracle length 17 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; call Query ; mov ecx,[esi+20h] ; sub ecx,eax ; mov eax,ecx ; pop esi ; ret
// The difference between the long at +0x20 and a no-argument __thiscall query @ 0x00C1E9C0 on the
// same object (the manifest calls the callee CWorldMap::GetNextRegionOnRouteTo, a propagated label).
struct CFrameCounter_Retail {
    unsigned char _pad_0x00[0x20];
    long          EndFrame;                 // +0x20 (name from use)
    long Query();                           // 0x00C1E9C0
    long FramesRemaining();
};

long CFrameCounter_Retail::FramesRemaining()
{
    return this->EndFrame - Query();
}
