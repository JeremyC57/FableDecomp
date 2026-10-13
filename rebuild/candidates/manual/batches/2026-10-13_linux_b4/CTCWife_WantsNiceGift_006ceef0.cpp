// CTCWife::WantsNiceGift()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEEF0, oracle length 44 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
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

extern long g_FramesPerSecond;         // 0x01375550 (.data)
extern long GFGetCurrentFrame();       // 0x0049D870

struct CHeroMarriageDefCalls {
    unsigned char _pad_0x00[0x48];
    long          SecondsBetweenReceivingGiftOpinionReactionCanHappen;   // +0x48 (PDB)
};

struct CTCWife_Retail {
    unsigned char          _pad_0x00[0x0C];
    CHeroMarriageDefCalls* PHeroMarriageDef_Object;     // +0x0C (PDB)
    unsigned char          _pad_0x10[0x74];
    long                   FrameLastReceivedNiceGift;   // +0x84 (PDB +0x88)
    bool WantsNiceGift();
};

bool CTCWife_Retail::WantsNiceGift()
{
    CHeroMarriageDefCalls* def = this->PHeroMarriageDef_Object;
    return GFGetCurrentFrame() - this->FrameLastReceivedNiceGift >
           def->SecondsBetweenReceivingGiftOpinionReactionCanHappen * g_FramesPerSecond;
}
