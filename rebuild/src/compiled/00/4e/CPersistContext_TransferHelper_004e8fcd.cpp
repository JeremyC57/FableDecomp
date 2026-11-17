// CPersistContext::TransferHelper(CWeaponTrailGraphicSet* value) -- CheckCRC(""), then in MODE_SAVE_BINARY write the set's two longs to the save stream, in
//   MODE_LOAD_BINARY read them through a CPersistTraits<CWeaponTrailGraphicSet> local (its DefaultVal built by CWeaponTrailGraphicSet's constructor:
//   Attack = Knockdown = -1)
// Leaf: ?TransferHelper@CPersistContext@@QAEXPAVCWeaponTrailGraphicSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E8FCD, length 84 bytes.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), CDataOutputStream::WriteSLONG @ 0x00993EB0 twice (the save stream +0x28 kept in esi),
// CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn @ 0x004E9021 (`this` = the traits local, the load stream +0x24 and the value on the stack). Shape: the
// traits local's two -1 words are written (`or [ebp-8],-1 ; or [ebp-4],-1`) before the mode ladder (`dec ; dec ; je ; dec ; jne`: cases 2 and 3); edi is saved only
// around the save arm. The chunk label "CEngineLightingManager::UpdateShadowScene" is propagated from an unrelated body.
// NF70 (round 7, land1117fu q10808): re-authored on engine/CPersistContext.h + CPersistTraits.h like its sibling 0x004E9550 (NF62,
//   CPersistContext::TransferHelper(CObjectAugmentationParticleSet*)); the stand-ins SBytePair / CPairReader_004e9021 and the local CPersistContext model
//   are gone. Types: Ego_r.pdb CWeaponTrailGraphicSet (Attack +0, Knockdown +4, longs; FableWin.pdb ??0CWeaponTrailGraphicSet@@QAE@XZ, inline here; no
//   shared header models it) and CPersistTraits<CWeaponTrailGraphicSet> (one member, DefaultVal; FABLE_RETAIL_CPERSISTTRAITS gives its constructor), whose
//   TransferBinaryIn 0x004E9021 is Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@VCWeaponTrailGraphicSet@@@@QAEXAAVCMemoryDataInputStream@@
//   AAVCWeaponTrailGraphicSet@@@Z. The row's own name is DESCRIPTIVE (no PDB names a one-argument transfer; spelled as the sibling; was the made-up
//   TransferBytePair_004e8fcd(SBytePair*)); overlay opt-in FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_WEAPONTRAILGRAPHICSET declares it.
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_WEAPONTRAILGRAPHICSET
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
class CWeaponTrailGraphicSet { public:          // Ego_r.pdb, 8 bytes; no shared header models it
    CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) { }
    long Attack;            // +0x00
    long Knockdown;         // +0x04
};
#include "engine/CPersistTraits.h"

void CPersistContext::TransferHelper(CWeaponTrailGraphicSet* value)
{
    CheckCRC("");
    CPersistTraits<CWeaponTrailGraphicSet> traits;
    switch (Mode) {
    case MODE_SAVE_BINARY: {
        CDataOutputStream* stream = PSaveStream;
        stream->WriteSLONG(value->Attack);
        stream->WriteSLONG(value->Knockdown);
        break;
    }
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, *value);
        break;
    }
}
