# Chunk 645 -- 0 of 10 landed (2026-10-31)

Cut from the parked-row pool before the re-rank (all ten are same-length residues). Every baseline was reproduced, one change per
grade; all ten stay DIFFER in `pending/2026-10-27_linux_hand645/differ/` (residue on line 2): 00b847d0 RadixSort one `xor ebx,ebx`
position after the histogram and scatter loops were fixed by a named `radix` local and `counts[radix] = counts[radix] + 1`; 00870e00
CActionEventTriggerParticle::SetDataFromString one register (a pointer named inside the branch moved the split copy into the branch);
004888c0 CPlayer::SwitchPlayerCharactersWith ebx/ebp swap; 00ba50a0 SetupStaticBlocks `failed = false` store position vs the dead
argument slot; 004feec0 UnloadRegion ebx/ebp swap; 0095b820 Spawn filter-store scheduling; 00853670 OnInitLeaf two store/reload slips;
00872110 UpdateAutoGoBehind one x87 operand pair (dot-product order does not move with spelling); 00838ec0 GetShotHitParameters
temporary slot + vector-copy register rotation (respelled to the PDB: const method, NHitLocations::CHitLocation*, CThing& shot, reference
setters, EDamageAttribute -- still carries an inherited `#pragma pack(push,1)` to replace before landing); 0087bd70 CRotateAround::Update
frame 0x44 vs 0x40. Lessons: a named key local plus `a[k] = a[k] + 1` fixes a radix histogram's register layout; a pointer named inside a
branch moves a split copy into that branch (try on the twin 0x00871290); a named local for a member read after a call reuses the dead
`this` register; when two bool locals compete for a dead argument's slot the one defined after the argument's first load is eligible and an
address-taken bool wins; dot-product x87 operand order does not change with spelling, park on sight. 4 NAME_FIXES + 4 review rows queued
(prefix c645): the CHitParameters setters SetHitLocation/SetWeapon/SetThingHitting take references, AddDamageWithAttribute(EDamageAttribute,
float); labels 0x00815280 (NHitLocations::CHitLocation::IsToCauseDecapitionOnHit, two-load getter: label_trust first), 0x00868590 /
0x008685F0 / 0x00868CA0 are the CHitParameters setters; the shot TC getters 0x007BED70/0x007BED80/0x007BEE20 unproven; same-TU stand-ins
for 0x0050E540 / 0x008E3EB0 in the 004feec0 / 0095b820 differs to review. Labels: ghidra_out/labels_chunk645_2026-10-27.tsv.
