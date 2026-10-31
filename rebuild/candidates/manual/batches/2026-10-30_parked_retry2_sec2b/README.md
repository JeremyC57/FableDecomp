# Parked-row retry sweep, wave 2 section 2b (2026-10-30/31): 12 of 337 held section-2 rows landed, 53 improved

Section 2b = the 337 rows section 2 held after a mechanical flag sweep (retail 0x00647575-0x007805D0), each now given a
hand attempt. Two container restarts killed the agent; the resumed agent continued from STATUS.tsv and stopped at the 8-hour
lane limit with 252 rows attempted (1-9 graded edits each) and 85 rows `NOT_ATTEMPTED_TIME` (section 2c). Landed 12, all
PASS (MATCH / RELOCATION_MATCH), all plain C++: 0x006AFF40, 0x006B0020, 0x006D7080, 0x006E2E60, 0x00717710, 0x007197C0,
0x0072ABD0, 0x00736760, 0x00751DA0, 0x00752300, 0x0075FE30 and 0x0077FDE0 (CPersistContext::Transfer<EHeroCentreDoorType>
graded over its true 208-byte extent; the 32 bytes at 0x0077FEB0 are the 2-argument Transfer, BOUNDS_FIX row recorded by
section 2). Four byte-matching bodies (0x006C9350, 0x006F3570, 0x006F37B0, 0x00702E00) are held in
`../../pending/2026-10-30_parked_retry2_sec2b/name_review/` because const-T template instances would compile to the same
bytes; two PENDING_HEADER rows (0x0071D940, 0x00698A30) need `CDefPointer(ENullDef)`. 53 improved sources in `../../pending/.../differ/`;
183 held with residues in STATUS.tsv (register tie-breaks, locals packed into dead argument slots, callee-saved pushes
sunk into one branch, a member kept in a register across a call, byte-register constants, folded bit tests, a float kept
in memory across a call).
Levers (matches / improved): stepwise float sums in permuted order 1 / 4; named float result of a difference 2 / 0;
post-increment on a reverse_iterator 1 / 0; true-extent grading 1 / 0; PDB inline accessor instead of a hand-walked chain
1 / 0; caller-hoisted map-cell byte offsets 1 / 0 (+2 name_review); `__forceinline` on a small inline emitted out of line
0 / 2; ~25 single improvements listed in NOTES.md. LESSONS.md has 11 new rules.
NAME_FIXES.tsv: 6 rows (CTCSpecialAbilities::FadeOrKillEffect static __fastcall, CBaseIntelligentPointer::GetPItem,
CMainGameComponent::GetCurrentServerFrame 0x00416225, CTCSoundPlayer::StopAttachedSound, CTCWeapon::IsMeleeWeapon is
0x005DAA00 and the landed 0x005DAA70 is NOT it) + 6 coordinator review rows in the queue (name_review quartet; g_MapwhoLevels
0x0137DADC vs 0x0137DAD8; CTCPhysicsBase vslots 72/73 spellings; 0x007338A0 OnBlockHoldExpired vs UnsetBlocking;
0x00681790 / 0x006817C0 identical twins; the CDefPointer header gap).
Review notes: stand-in GetSubDef bodies in the 0x00739360 / 0x00680050 / 0x00670D50 sources (differ) want the real noinline
`GetSubDef<T>`; 0x00704860 uses a local CCountedPointer instead of engine/CCountedPointer.h; SHARED_HEADER_AVAILABLE on
0x00717710, 0x00736760, 0x00751DA0, 0x00752300, 0x006AFF40 / 0x006B0020; `__CIfmod` import-thunk alias in three rows.
Coordinator: vland --land 12/12 (one RELOC_CONFLICT warning: g_MapwhoLevels, queued), bindings_multi_check 0, no RELOC_IDENTITY.
