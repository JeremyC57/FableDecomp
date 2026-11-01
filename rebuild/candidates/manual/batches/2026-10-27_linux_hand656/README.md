# Chunk 656 -- 10 of 10 landed (2026-11-01, NA002 of the never-attempted pool: landed-twin rows)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA002); each row's masked body equals a landed row, the twin's source was
the starting point and every row's own identity was settled from its caller chain first. Landed via `coordinator/land_chunk.sh 656`
(vland --land 10/10 RELOCATION_MATCH + PASS, multi-check 0; 9 rows on the first probe): 0x00CD4550 TeleportData::~TeleportData,
0x00CD4FA0 std::_Destroy<TeleportData> (COMDAT order), 0x0078ADF0 ~CCreatureAction_FireMissileWeapon, 0x0086D530 the non-const
__uninitialized_copy<CTextureMorphMap*>, 0x004C2DF0 ~C3DAnimationBlendState, 0x005E0A80 ~pair<const CCharString,
CIVCountedPointer<CAnimComponentBase> > (symbol tentative among three identical copies), 0x00F270D0 ~CHeroSoulsCreatureDef,
0x00487550 CPlayer::PrepareForLoad, 0x007F8140 CTCActivationReceptorDoor::Deactivate (slot 29), 0x008D69A0
NCreatureMode::CModeGuildmasterPowerUpHeal::OnDeactivate (slot 6). No row parked.
10 NAME_FIXES queued (c656). Labels: ghidra_out/labels_chunk656_2026-10-27.tsv (18). Review at landing: the three-copy ~pair
order rule (0x005E0A80 / 0x005E0AB0 / 0x005E1790, also 0x005E0160 / 0x005E0C00) and the ~27 stand-in bindings on 0x00A01C10;
header debt: CTCActivationReceptorBase, CModeStandBase, CClass and CCreatureAction headers flatten the vptr (local polymorphic
models with PDB member names); CCharString local (fable_string.h carries `#pragma pack`). Lessons in GOTCHAS (1 line) and LESSONS.md.
