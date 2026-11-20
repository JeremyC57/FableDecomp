// CPersistContext::Transfer<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(const char* name, ArcheryOpenState& value, const ArcheryOpenState& defaultValue) -- persist one enum member ("ArcheryStateCurrent"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: ??$Transfer@W4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@@CPersistContext@@QAEXPBDAAW4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@ABW4123@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDCC20, `ret 0Ch`. Called from NScript::CQ_SunnyvaleMasterData::OnPersist 0x00CDC070 with the key
//   "ArcheryStateCurrent".
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0 (catalogued "WriteCBYTE"),
//   CPersistTraits<ArcheryOpenState>::TransferBinaryIn 0x00CDCD00, CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<ArcheryOpenState> wrapper, vtable 0x012C2ECC:
//   slot 0 TransferOut 0x00CDCDB0, slot 1 TransferIn 0x00CDCD90).
// Name-fix NF163 (q4631 / q11756, 2026-11-20): Ego_r.pdb 0x00CC18B0 + FableWin.pdb 0x03B4E0B0 spell this instance
//   ??$Transfer@W4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@@CPersistContext@@QAEXPBDAAW4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@ABW4123@@Z,
//   and the landed binder 0x00CDC070 already binds it. Was a local model with the descriptive global `enum EArcheryState` (leaf
//   `??$Transfer@W4EArcheryState@@@CPersistContext@@...`); now on the shared headers: the member template body of engine/CPersistContext.h
//   (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY, the function-scope traits variant FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE, which
//   the landed row had: the traits are built once before the jump), engine/CPersistTraits.h, engine/CPersistContext_TransferableComponent.h
//   and the PDB enum NScript::CQ_SunnyvaleMasterData::ArcheryOpenState (engine/CQ_SunnyvaleMasterData.h).
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CCHARSTRING
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CQ_SunnyvaleMasterData.h"

template void CPersistContext::Transfer<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(const char* name,
    NScript::CQ_SunnyvaleMasterData::ArcheryOpenState& value, const NScript::CQ_SunnyvaleMasterData::ArcheryOpenState& defaultValue);
