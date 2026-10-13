// CTCWife::WantsNiceGift()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEEF0, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; mov ebx,[g_FramesPerSecond] ; push esi ; mov esi,ecx ; push edi ; mov edi,[esi+0Ch] ; call <frame counter> ; sub eax,[esi+84h] ; mov ecx,[edi+48h] ; imul ecx,ebx ; xor edx,edx ; cmp eax,ecx ; pop edi ; setg dl ; pop esi ; mov al,dl ; pop ebx ; ret
// engine/CTCWife.h (PDB): +0x0C PHeroMarriageDef_Object, +0x88 FrameLastReceivedNiceGift (retail
// +0x84: the GiftsReceived map before it is 4 bytes smaller). PDB CHeroMarriageDef +0x48 =
// SecondsBetweenReceivingGiftOpinionReactionCanHappen. A wife wants another nice gift once more
// frames than that many seconds (at the global frame rate, long at 0x01375550) have passed
// since the last one. The frame counter is the global at 0x0049D870 (__cdecl).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.
//
// Lane feedback: the frame-rate global must be read into a local before the call (retail holds it in ebx),
// and declared AFTER the def local, or the callee-saved registers swap. RELOCATION_MATCH.
extern long g_FramesPerSecond;         // 0x01375550 (.data)
extern long GFGetCurrentFrame();       // 0x0049D870

struct CHeroMarriageDefCalls {
    unsigned char _pad_0x00[0x48];
    long          SecondsBetweenReceivingGiftOpinionReactionCanHappen;   // +0x48 (PDB)
};

// Retail CTCWife view (FrameLastReceivedNiceGift is PDB +0x88; see the README container rule).
struct CTCWife_Retail {
    unsigned char          _pad_0x00[0x0C];
    CHeroMarriageDefCalls* PHeroMarriageDef_Object;     // +0x0C (PDB)
    unsigned char          _pad_0x10[0x74];
    long                   FrameLastReceivedNiceGift;   // +0x84 (PDB +0x88)
    bool WantsNiceGift();
};

bool CTCWife_Retail::WantsNiceGift()
{
    // Enough frames since the last nice gift (def seconds * frame rate)? Both operands are read
    // before the frame query: def into edi, the frame rate into ebx (retail's register order
    // follows the declaration order: def first, then the frame rate).
    CHeroMarriageDefCalls* def = this->PHeroMarriageDef_Object;
    long framesPerSecond = g_FramesPerSecond;
    return GFGetCurrentFrame() - this->FrameLastReceivedNiceGift >
           def->SecondsBetweenReceivingGiftOpinionReactionCanHappen * framesPerSecond;
}