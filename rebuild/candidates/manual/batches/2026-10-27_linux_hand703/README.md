# Chunk 703 (NA046) -- 10 of 10 byte-exact; all ten labels wrong or placeholders, two unaligned rows proven real starts (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 12 probes in all; no asm, volatile, flags or pragmas):
CTCSkeletalMorph::Copy 0x00786650 (slot 12 = CTCBase::Copy; tail-jumps to the landed vector<CSkeletalMorph>::operator=),
CTCActivationReceptorDoor::Activate 0x007F8120 (slot 28; slot 29 is the landed Deactivate), CThingDeadCreature::WorldUpdateBulletTime 0x00836240
(slot 20; the catalog's RenderSubPrimitive was wrong), the CAIStateGroup_MinionReactToCrime / CAIStateGroup_VersusScorpionKing ctors 0x00900B41 /
0x0090143E (each stores its own RTTI vtable; CIntelligentPointer at odd offsets +0x1B / +0x1D per the PDB, pack(1) on the pointer type; factory sizes
0x24 / 0x28 = PDB sizes), NGraphicAppearance::CTCGraphicAppearance::OnKill 0x004BF8F0 (slot 14; real STLport list loop), NUISystem::CScrollBar::OnLeftClicked
0x005596D0 (slot 146), the const CAnimComponentCombatMisc::Alloc 0x005DF750 (slot 4; new 0xA = PDB sizeof), and the byte-twin OnKill overrides of
CTCInventoryMap 0x005FAEE0 / CTCInventoryQuests 0x0061A1F0 (each named from its own vtable: two symbols, no conflict). 0x00900B41 / 0x0090143E sit in a
TU packed without padding; the function before each ends in `ret 4`, so they are real function starts, not fragments.

17 labels applied (`ghidra_out/labels_chunk703_2026-10-27.tsv`; the 0x013B8790 data label kept in the naming wave 18 spelling); NAME_FIXES.tsv 17 rows +
one coordinator decision row queued c703 for naming wave 31: CThing::WorldUpdateBulletTime 0x004C8BA0 is virtual (bound QAE), the CAIStateGroupBase /
CBaseIntelligentPointer base ctors 0x008FD255 / 0x00A01B10 (stand-ins CBase_008fd255 / CMemberBase_00a01b10 and ~20 invented spellings across ~70 landed
generated ctors), the landed 0x00900B81 caller's ??0CAIStateGroupBehaviourBase@N_00900b41@@, CTCGraphicAppearance::ClearAllPrimitives 0x004BF9A0 protected
(IAE, both PDBs), the STLport _List_base<CPrimitive>::clear 0x004C5690 (allocator is class-keyed), NInventory::CTCInventoryBase::OnKill 0x005BC9C8,
GetPHeroInfoScreensManager 0x00437CD0 and the PClass singleton 0x013B8790 (many invented spellings each). Coordinator decision row: 0x004C7590 has seven
landed spellings and no PDB name (SetGraphicAppearanceChanged / UpdateGraphicAppearance / ForwardToGraphicAppearance / RefreshGraphic / SetActive x2 /
Method_); the wave settles one from Ego_r order and callers. Header debt (NOTES.md): CTCBase.h / CTCActivationReceptorBase.h flatten the vptr, CThing.h is
Ego_r-offset, CAnimComponentCombatMisc.h flattens its base, CIVCountedPointeeBase.h's retail branch lacks the FableWin default ctor; CManager / CPlayerGui
headers are other classes' layouts (local models, as the landed twins). Unverified lead: 0x005DF770 stores vtable 0x01255760 (probably the
CAnimComponentCombatFlourishTargetOffset ctor).
