# Chunk 702 (NA045) -- 10 of 10 byte-exact; all ten labels wrong or incomplete, five vfunc_N rows named (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 14 probes in all; no asm, volatile, flags):
NUISystem::CParticleEffect::~ 0x00543CF0 and NUISystem::CSwappingStateComponent::~ 0x005474C0 (each stores its own vtables; its ??_G inlines the same
body), NCreatureMode::CModeOutOfActionBase::~ 0x008D7580 (pack(1): PDB PEmitter at offset 109, as the landed ctor 0x00740160), the private
CTCHeroOnlineScoreboard::ClearScoreboard 0x00562E10 (real STLport map / _Rb_tree / list), the implicit
CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~ 0x00BFBBF0 (`virtual __forceinline ~CEnginePrimitiveHandle()`, the landed includers' model; Ego_r has
no out-of-line copy), and the five vfunc_N rows: NUISystem::CSpellContainerList::AddSpellSelectionChild / AddSpellIndicationChild 0x00544310 / 0x00544330
(slots 142 / 143), NPlayerGui::CDrawAbilityButtonsPC::GetAssignableSpecialMoveIndexForButtonDef 0x00657200 (slot 3; the interface pointer in a named
local), CTCSmokeGenerator::Copy 0x00722570 and CTCFamilyMember::Copy 0x00778B10 (slot 12 = CTCBase::Copy). Decisions (coordinator): the __forceinline
handle dtor and the pack(1) creature-mode model are the established landed conventions (accepted).

19 labels applied (`ghidra_out/labels_chunk702_2026-10-27.tsv`); NAME_FIXES.tsv 20 rows queued c702 for naming wave 31: NPlayerGui::CDrawBase::GetPlayer
pair 0x00643D7C / 0x00643D84 (landed as _global::Forward_GetMainPlayer_via0x8_*; this chunk binds the const one by its PDB name, so the own row's respell
comes first in wave 31 -- one address, one name), CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef 0x00643FC8 (landed with an invented
name), the const CGamePlayerInterface member 0x00445EE0, NParticleEngine::CParticleEmitter::Kill 0x00AEE8C0, the scoreboard STLport members 0x00569A80 /
0x00566DC0 / 0x00566E10, CTCDoor::Copy 0x007305A0 (leaf on an invented CTCDoor_Methods), the private ClearScoreboard binder 0x00562E70 and the
CModeStunned / CModeDazed _Destruct stand-ins of 0x008D7580. 0x00562E70 as CTCHeroOnlineScoreboard::Close is only probable (donor order + body, no
RTTI): queued, not labelled. Header debt (NOTES.md): CTCBase.h, CModeStandBase.h, CObserver.h, CParticleEmitter.h (global flat struct; retail
NParticleEngine::CParticleEmitter), CEnginePersistentPrimitiveBase.h flatten or misplace their class; CGamePlayerInterface.h is data-only;
CIVCountedPointeeBase.h's retail branch uses FABLE_STATIC_ASSERT without including rebuild_abi.h.
