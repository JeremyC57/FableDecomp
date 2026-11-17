// CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value) -- persist one particle set: CRC-check the empty key, then run CPersistTraits<CObjectAugmentationParticleSet> in the context's direction (MODE_LOAD_BINARY load, MODE_SAVE_BINARY save)
// Leaf: ?TransferHelper@CPersistContext@@QAEXPAVCObjectAugmentationParticleSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9550, length 77 bytes (`ret 4`). Callees: CPersistContext::CheckCRC
// @ 0x00404500 with "" @ 0x0122D70E, the traits object's DefaultVal constructor ??0CObjectAugmentationParticleSet@@QAE@XZ @ 0x004E9627,
// CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn @ 0x004E9642 / TransferBinaryOut @ 0x004E959D (Ego_r.pdb / FableWin.pdb
// by-reference spellings), ~CObjectAugmentationParticleSet @ 0x004E25F1. `this` is the persist context (Mode +0x18, PLoadStream +0x24,
// PSaveStream +0x28); the 0x2C-byte frame is the traits object (its DefaultVal). Frame-pointer size TU:
// #pragma optimize("y", off) + ("s", on). The name TransferHelper is descriptive (no PDB names a one-argument transfer of this type).
// Shape: the switch arms RETURN (the traits destructor still runs at each exit): VC7.1 then forms the traits `this`
// once above the first `je` and the default arm falls straight into the destructor call; with `break` every arm re-forms
// `lea ecx,[ebp-2Ch]` (+3 bytes).
// NF62 (2026-11-17, round 7): ported onto the shared CPersistContext / CPersistTraits headers (the local CPersistContext model and the
//   CPersistTraits_ParticleSet stand-in are gone). Overlay opt-ins: FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_PARTICLESET (the
//   declaration) and FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET (the explicit traits specialisation).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_PARTICLESET
#define FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

void CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value)
{
    CheckCRC("");
    CPersistTraits<CObjectAugmentationParticleSet> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, *value);
        return;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, *value);
        return;
    }
}
