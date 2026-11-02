# Chunk 700 (NA043) -- 10 of 10 byte-exact; all ten catalog labels were wrong (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 1-2 probes each; no asm, volatile, flags or
inline_depth). Seven "CTCInventoryItem::OnDie" rows are destructors: the implicit ~CThingCreatureDef 0x00671100 (the whole def-class chain expands in
one ??1), NParticleEngine::CPSCAttractor::~ 0x00B18780, ~CGameMap 0x006B7570, the non-virtual CCombatActionBase::CStrikeEvent::~ 0x0085AD00,
~CMirroredTextureHighlightsChunk 0x00A9D330, ~CSoundEmitterLH 0x00AAC4F0 (`{ Shutdown(); }`, real STLport list) and CDefinitionManager::CDefClassInfo::~
0x009B5D50 (four uncalled rows named from FableWin's ctor / ??_G-then-??1 neighbour pairs mirrored in retail). The three "CVillageTaskCarryCrate::
AssignCreatureToTask" rows are three setters: CTCTargetingBase::SetPNearestInterestingThing 0x007F5810 (protected), CTCTargetingBase::
SetPScriptOverrideRangedTarget 0x008ABB50, NCreatureMode::CModeScorpionKingVulnerable::SetTarget 0x008D73E0 (by reference, per the PDB). Four implicit-dtor
rows carry the DestroyOne helper (accepted). `#pragma pack(1)` on the CIntelligentPointer classes and the defs through CThingBaseDef models retail's
own packing (members at +0x0D / +0x1D / +0x7D; Ego_r offsets), the documented practice (GOTCHAS chunks 136 / 164; 2,997 landed sources).

22 labels applied (`ghidra_out/labels_chunk700_2026-10-27.tsv`); NAME_FIXES.tsv 22 rows + 2 coordinator rows queued c700 for naming wave 30: the
ScorpionKing / ranged-target binders (no namespace, pointer for reference; the invented CTCTargeting_Retail), the CSoundEmitterLH family (0x00AAC2E0
landed as CAnimQueueOwner00aac2e0::StopAllQueuedAnims, its list clear 0x00AACC60 under three spellings, RemoveInactiveSounds 0x00AABCA0), the
CBaseIntelligentPointer members 0x00A01B10 / B50 / B90 / C10 (21-56 invented spellings each), stray bindings of ~CBaseClassNonCopyable / ~CBaseClass /
~CChunkedFileChunk / ~CCharString / ~CResource. Decisions (coordinator): 0x00AAC2E0 keeps the one bound descriptive spelling CSoundEmitterLH::Shutdown
(the class is in neither PDB), virtual once the vtable is modelled; the landed 0x004F9CFC (probably ~CCardPositionsDef) is queued for proof, not
renamed. Header debt (NOTES.md): CParentDefClassBase.h declares an out-of-line dtor where retail's is implicit; CModeStandBase.h / CTCBase.h /
CMirroredTextureHighlightsChunk.h flattened; CDefinitionManager.h lacks the nested CDefClassInfo (vland SHARED_HEADER_AVAILABLE on 0x009B5D50).
