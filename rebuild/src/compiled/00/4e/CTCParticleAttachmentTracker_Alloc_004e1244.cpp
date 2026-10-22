#pragma optimize("s",on)
// CTCParticleAttachmentTracker::Alloc(CThing&) -- factory: new CTCParticleAttachmentTracker(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1244, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCParticleAttachmentTracker::CTCParticleAttachmentTracker ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCParticleAttachmentTracker) = 0x10 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DED6F on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CTCParticleAttachmentTracker;

struct CTCParticleAttachmentTracker {
    unsigned char _body[0x10];       // sizeof(CTCParticleAttachmentTracker) = 0x10
    CTCParticleAttachmentTracker(CTCParticleAttachmentTracker* owner);                   // 0x004DED6F
};

CTCParticleAttachmentTracker* __fastcall CTCParticleAttachmentTracker_Alloc(CTCParticleAttachmentTracker* owner)
{
    return new CTCParticleAttachmentTracker(owner);
}