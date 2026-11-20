# NF199 notes (round 10, remaining queue, entries q4389 .. q12144; handed back for context after q6341)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- 50 entries: 0 done, 1 done-already (q6314), 6 blocked (q4389 / q4451 / q4456 partial; q4394 port; q6332 / q6341 family),
  2 deferred (q6315, q6319: held by NF198), 41 open (not started, from q6333 on).
- Rows: 27 ported (RELOCATION_MATCH + PASS, one recheck_final.log run), 0 not-a-binder, 17 blocked, 1 deferred (0x009BDE70, NF197 lock).
- selfcheck.log: 0 problems. No header overlay (hdr/ empty; hdr/README.md).

## Descriptive names
- None new. Every respelling moves a binder onto a spelling that already exists: the self Leaf of 0x006F0250 (owner decision D1
  member-template _M_find), the self Leaf of 0x00411570 (Ego_r.pdb protected static CBasicString<char>::Compare) and Ego_r.pdb
  ??0CCharString@@QAE@XZ for 0x0099E4B0 (bound by 712 landed rows; the self row has no Leaf line).
- Leaf lines rewritten from a bare name to the full mangled symbol (same symbol as the HEAD object, objdump-checked; no self rename):
  0x006E9960, 0x006EB390, 0x006A7F30, 0x00A14EE0; added (none at HEAD): 0x006EB600.

## Splits
- No address grows a spelling. 0x006F0250 loses CCombatantMap::FindNode / SCombatantMap::FindNode / CCombatantMap::find /
  CRetailRbMap_UID_CombatantInfo::find (still bound by the blocked rows listed in APPLIED q4389) / CombatMap::FindLE (gone).
  0x0099E4B0 loses ??0CCharStringMember@@QAE@XZ (all 14 binders moved), ??0CMember_0099e4b0@@QAE@XZ and both
  CClass_009a9xx0::Initialise stand-ins. 0x00411570 loses ?Compare@@YIJPBD0@Z at 0x0099E900 and ?Compare_00411570@@YIHPBD0@Z at two
  rows (both stand-ins remain bound by other q4451 rows).

## Pick-any copies (advisor rule)
- 0x0099E900 / 0x006A7F30 / 0x00A14EE0 newly define ??8?$CBasicString@D@@QBE_NABV0@@Z (table: 0x004A93C0): cmpbody against the owner
  object = 34 bytes, bytes and relocations equal. They also define ??8CCharString@@QBE_NABV0@@Z (no table line; inline in retail).
- The q4389 rows on MFIND_DECL emit no _M_find copy; their new COMDATs are header inlines (iterator ctor / -> / *, tree and map find)
  with no table line. 0x006EA770's NOINLINE draft (not handed back) would have emitted _M_find + less<_K> (table 0x00526120).
- The q4456 rows define no new symbol.

## Tests
- 0x006A7F30 / 0x00A14EE0: drafted with FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ (the local operator was forced); the plain form
  (FABLE_RETAIL_CCHARSTRING only) was then graded, also RELOCATION_MATCH with the same pick-any copies, and is the one handed back.
- 0x006E9360 / 0x006E9390: the landed tests linked the row and defined the SCombatantMap::FindNode stub (LINK_FAIL after the port);
  rewritten on stlport/fable_map.h in the NF195 0x006E9270 shape (PassPattern kept). Every other test is a standalone host model and is
  unchanged. (The 0x009A0EC0 test still names host statics g_StreamingFontAlphaMaskUV0/1: not a binding, left alone.)

## Pre-existing warnings (not touched)
- STANDIN_LEAF on the _unattributed CStringOwner_* / CClass_* self rows (placeholder-family naming is paused).
- RELOC_NAME: 0x004D3826 ??0CBase_0044c0c0 (0x0044C0C0 CSubDefClassBase ctor), 0x006E97F0 ??ECAttackerIterator (0x006EF790),
  0x006ED9E0 ?IsCombatant@CThingCreatureBase (0x00662430 IsInMeleeAsAttacked).
- SHARED_HEADER_AVAILABLE local models (CCombatManager, CThingCreatureBase, ...) were already in those rows at HEAD; no class token was
  added (selfcheck). 0x006ED9E0 drops its local CCombatantInfo_Retail for engine/CCombatantInfo.h.

## Header items / coordinator
- q6332 / q6341 (one unit, same address; the queue resumes at q6333): one probe of the self row 0x00433530 on the shared headers gave
  DIFFER(280v316); the cause is NOT established and the existing fable_containers.h opt-ins (FABLE_STLP_OUTOFLINE_UNINIT_COPY /
  _FILL_N, the NF41 block at 421-445) were not tried. No header claim is asked for. Four-row family (0x00433530, 0x004333B3,
  0x004B4A10, 0x004B2FE0).
- Stale table line: reloc_bindings.tsv names 0x006E9360 as the witness of ?FindNode@CUIDMap@@QBEPAUSMapNode@@AB_K@Z (0x006F0250), but
  its HEAD object binds ?FindNode@SCombatantMap@@QBE..; no landed row declares CUIDMap (source grep: comments only).
- No cast of a stand-in to a real type was added.
- Funclets: none of the 27 changed rows is #included by another landed row (grep of each file name); no self rename, so no file moves.

## Locks
- Claimed from `released`: 0x006E9310 0x006EA050 0x006ED270 0x006EDA80 0x006E9390 0x006ED9E0 0x006E93C0 0x006E9700 0x0099E900.
  Claimed new: 0x006A7F30 0x00A14EE0 0x00413280 0x0044A570 0x0047A750 0x004CB0D0 0x0050B470 0x00525FF0 0x0074C6D0 0x00822690
  0x00869FA0 0x009C97A0 0x00BF8D50 0x00CB7840 0x00CD4580 0x004D3826. Not touched: 0x009BDE70 (NF197), 0x00833A70 (NF198).
- Locks still held by NF199 on rows NOT changed (blocked / not attempted; the coordinator may release them): 0x006E9310 0x006EA050
  0x006EA770 0x006EAC50 0x006ED270 0x006EDA80 0x006ED510 0x006E93C0 0x006E9700 0x009F03B0 0x00433530 0x004333B3 0x004B4A10 0x004B2FE0
  0x009CB910 0x00A16D50 0x006D41F0, plus the rows.txt locks of entries left open.

## Rule slips
- One Bash call exited non-zero (it ended in a `diff` that found differences, the 0x00406070 transformer check).
- Brief reading list: NF92 BRIEF.md, docs/pipeline/ASM_RULES.md and the NF44 "Hand-back" section were not read before starting (the
  NF92 LESSONS / NOTES, NF44 sections 73-192, NF88-90 LESSONS, GOTCHAS, EXTRA_RULES were). No asm token was written (selfcheck 0).
- 0x00433530 is marked blocked after one form, not three (hand-back for context); STATUS says so.
- q6341 was taken with q6332 ahead of q6333 (same address, one unit).
- grades_used was reconciled with grades.log + every vland payload at hand-back (it counts HEAD compiles for symdiff, q.py grades and
  vland runs, including the final recheck).
