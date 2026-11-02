# Chunk 699 (NA042) -- 10 of 10 byte-exact; eight "CTCInventoryItem::OnDie" labels were seven destructors and the real OnDie (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 1-2 probes each; no pragma, flags, asm or volatile):
the private CEnginePrimitiveRenderer2D::AddPrimitiveManager 0x00B4ABB0 (from its landed 3D twin 0x00B33AE0), CTCInventoryItem::GetInventoryOwner()
const 0x005D8E90, CTCInventoryItem::OnDie 0x005D8EC0 (vtable 0x01240004 slot 13 -- OnDie in CTCBase order, OnKill is 14; `RemoveFromInventory();
CTCBase::OnDie();`), and seven destructors labelled "CTCInventoryItem::OnDie": the implicit ~CFireHeartPatternDef 0x00D34F80 (the
engine/CDefClassBase.h retail opt-in), ~CHandleBasedMemoryPool 0x00A02DB0, the libpk ~CVtable_012bf0c8 0x00C775A0 (descriptive: libpk has no PDB or
RTTI; the landed CReverbProcessor rows' spelling), and the implicit ~C3DMeshFileClothChunk 0x00A9D3C0, NParticleEngine::CPSCSingleSprite::~ 0x00B0A160,
NScript::CV_FishingCompetitionScript::~ 0x00E81F30 and NCreatureMode::CModeSummonerSummon::~ 0x008CC330 (five DestroyOne helpers, accepted).

14 labels applied (`ghidra_out/labels_chunk699_2026-10-27.tsv`); NAME_FIXES.tsv 14 rows + 1 coordinator row queued c699 for naming wave 29: the landed
0x00A9D3A0 defines ?OnDie@CTCInventoryItem@@QAEXXZ but is ??1C3DMeshFilePrimitiveChunk@@UAE@XZ (a different mangled name from the new UAE OnDie, so no
symbol collision -- re-modelled in the naming wave, not here; its catalog label is fixed now), ~CChunkedFileSuperChunk 0x00AA8490 (bound
?OnDie@BaseItem), the STLport _List_base<CSpriteTrailEntry> dtor 0x00B09E40 (proved by grading the COMDAT from this object against its row),
CThingManager::PeekPThingWithUID 0x0051ED60 (landed as "CStopWatch::GetTicks"), the protected NInventory::CTCInventoryBase::RemoveItemFromInventory
0x005BC499, CTCBase::OnDie 0x006868E0 (bound vfunc_13), libpk ReleaseBlock 0x00C77470 (not a dtor). Decision (coordinator): 0x00C74450's three libpk
spellings go to one descriptive spelling per class in a libpk header pass (queued). Header debt (NOTES.md): CTCBase.h, CTCInventoryItem.h,
CModeStandBase.h, CHandleBasedMemoryPool.h, C3DMeshFileClothChunk.h, CChunkedFile*Chunk.h, CEnginePrimitiveRenderer2D.h, CEngineComponent.h,
CEnginePrimitiveTypeInfo.h flatten the vptr; CLevelScriptBase.h declares an out-of-line virtual dtor where retail's is implicit.
