// LHA::CEventPlayOptions::SetFlag(unsigned long index, bool set) -- set or clear bit `index` (of 8) in the 16-bit state mask at +0x4e and mark it in the touched mask at +0x2c through the shared __fastcall bit helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A8C0, length 24 bytes.
// Leaf: ?SetFlag@CEventPlayOptions@LHA@@QAEXK_N@Z
// Vendor naming wave V1 (one address, one name): 0x00C0AF00 ?SetMaskBit8@@YIXK_NPAG1@Z (was ?SetMaskBit8_00c0af00@@YIXK_NPAG1@Z)
// Callees: the __fastcall helper @ 0x00C0AF00 (ecx = index, dl = set, stack = the mask, then the touched mask; it
// forwards to SetMaskBit 0x00C0AE70 with a bit count of 8, which ORs `1 << index` into the touched mask -- the SECOND
// stack word, here +0x2c -- and sets or clears it in the FIRST stack word, here +0x4e). The chunk label
// "CTCLook::IsTurningBody" is propagated over byte-identical neighbours (its twin 0x00C0A8A0 uses +0x4c / +0x2a and the
// 0x00C0AEE0 helper); the callers (0x00AABE27 / 0x00AAC057) loop over bit indices with a stack object, so the class is
// named by address. Shape: a `__fastcall` member forwarding to a `__fastcall` free function -- the byte argument goes to
// dl, the member addresses are pushed right to left (+0x2c first), and the index is re-read into ecx after the pushes.
// Fix (sweep1, 2026-10-25): the mask roles were swapped in the comments and member names (FINDINGS chunk 431); the
// bytes push +0x2c (the helper's OR-only touched word) first and +0x4e (the set/cleared word) second, as the landed
// SetMaskBit 0x00C0AE70 reads them.
// Naming (agentNF31, 2026-11-12): class respelled CBitMaskPair_00c0a8c0 -> LHA::CEventPlayOptions (Ego_r struct_layouts_egor:
//   WhichGainSendFaders +0x2a, WhichPitchSendFaders +0x2c, Params (+0x30) GainSendFaders +0x4c / PitchSendFaders +0x4e = the
//   words this body passes; both callers build the object with ??0CEventPlayOptions@LHA@@QAE@XZ 0x00C0A790 and the landed
//   caller 0x00AABD50 already binds this spelling through engine/LHA_CEventPlayOptions.h). Method name stays descriptive
//   (no PDB carries LHA member functions).
#define FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS
#include "engine/LHA_CEventPlayOptions.h"

void __fastcall SetMaskBit8(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched);   // 0x00C0AF00

void LHA::CEventPlayOptions::SetFlag(unsigned long index, bool set)
{
    SetMaskBit8(index, set, &Params.PitchSendFaders, &WhichPitchSendFaders);
}
