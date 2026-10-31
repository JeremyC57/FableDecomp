# Chunk 630 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 630` (vland --land 3/3, multi-check 0): NCameraModes::CBase::UpdateRaiseUp
0x0084D7E0 (RELOCATION_MATCH; the catalog autoname CTCGraphicAppearance_UpdateHeightInterpolation was wrong -- Ego_r donor order lines
up 1:1; read only the Z of the by-value GetFocalPos result into an otherwise unused C3DVector local; observed thing via an inline
accessor on the camera manager -> `lea ecx,[eax+100h]`), CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed 0x0092F240
(RELOCATION_MATCH; the filter is IsOfType(0x14) && IsTargettedAtThing(creature), the FindMostRecentMessage template body compiled in
the same file as a real list walk, named CCharString for the sound name), CSB::GetDistanceBetweenAgents 0x00A872F0 (MATCH; named
float locals for the length and the other agent's radius move the byte test ahead of the fadd). All three use the shared headers
(CSB, CWorld, CAIStateGroupBase, CGameCameraManager) via the `#define X XLayout` include. Seven rows stay DIFFER in
`pending/2026-10-27_linux_hand630/differ/` (0053e0c0 edx/eax, 00ac82a0 prologue schedule vs ebx/ebp, 0078c150 frame 0xC v 8 after
the release fix 202->192, 00488620 / 004886f0 dead-filter frame word, 00632830 -- really GFSerialiseMapBinaryIn<long,ETableSprites>,
offsets 4 lower --, 0072b260 eax/ecx). 14 NAME_FIXES (message-filter ctors/operator() 0x006AF290 / 0x006AF2B0 / 0x006AF1E0 /
0x006AF1C0, template names 0x008D46D0 / 0x0090DEE0, GetPItem, landed 0x0084D8B0 namespace + virtual const, CThing::GetPlayer /
PeekPlayer, AddTC, CTCEnemy::AreThingsEnemies, GetFocalPos 0x004C1060) + 3 review rows queued (prefix c630). Lesson to retest on the
GOTCHAS "lane gap" releases 0x69A8B0 / 0x7366C0 / 0x729410: a def out-slot released by hand at function scope gives retail's
`dec [esi+4] ; jne`. Labels: ghidra_out/labels_chunk630_2026-10-27.tsv.
