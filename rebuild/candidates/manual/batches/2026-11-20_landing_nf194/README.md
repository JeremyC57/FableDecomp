# Landing nf194 (Account A, 2026-11-20): script interface / defs / quest script names, third pass (61 rows)

Agent NF194 (round 10) handed back early (context) after q11915 with 50 entries: 18 done, 5 done-already, 1 rejected, 7 blocked and
19 open (q11955 onward). Its files are in `naming/2026-11-20_agentNF194/`, with the held 0x00EE2A80 draft (`work/00ee2a80_held.cpp`).
The open entries and the GetSubDef<CSpecialEffectsDef> family (q11718 / q12068, 33 rows, NF170 census) and q11724's last row go back
to the queue unclaimed (22 entries).

- 61 rows land: 1 MATCH + 60 RELOCATION_MATCH, every test PASS.
- q5992: the 35 CEntityScriptBinding<T>::CreateActiveEntityScript rows move off per-row stand-ins onto engine/CCountedPointer.h. ShareData
  0x00CE1040 is bound under the PDB spelling (new declaration-only opt-in FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY: retail calls
  the out-of-line copy from these speed-optimised TUs). The self leaves change only in the per-row hidden-result namespace (stand-in to
  stand-in); no landed row binds those addresses.
  - 0x00CE1000: the rows' `~CCountedPointer<CEntityScriptBase>` copy is the header's destructor, which is the Leaf the landed owner row
    already carries. All 34 emitting objects equal the owner (53 = 53, `logs/pickany_check.txt`). The table there goes from 37 lines to 2
    (the owner's spelling, and 0x00D30000's stand-in from HEAD).
  - Advisor decision (owner delegated): this does not break the q5991 hold. These rows choose no name; they take the header's. If
    the 53-byte body proves to be ReleaseData, one header edit moves all 35. q5991 stays blocked, and its note now says so. The
    advisor also corrected the nf189 check (destruction sites prove nothing; assignment / Reset bodies and the Ego_r name sets
    discriminate); the queued debt line is rewritten.
  - None of the 35 rows calls the destructor explicitly. The four `~CCountedPointer()` hits are destructor definitions in row-local
    models already present at HEAD.
- Self renames (moved files listed in NOTES): 0x00F0D470 and 0x00D15140 become the CQ_AmbushTradersScript / CQ_BountyHuntScript
  TeleportData ctors (q6345 / q6365 / q6414, q6346 / q6366 / q6517). 0x00D15140's old leaf was also the real CMainGameComponentInit
  ctor's symbol (0x004131A0), so that LNK2005 leaves. Also 0x00D360E0 CheckGameEnding (q6412), 0x0088B7E0
  CAIBrain::GetCurrentStateGroupDebugText (q6436 / q6440), 0x00EA3490 / 0x00E0C350 / 0x00D1CAC0 / 0x00E0A510 (q6470 / q6491 / q6505 /
  q6529), q11668 (ABE access on 0x0089C100 and its caller 0x008A7EC0), and q6427 (catalog module only, PDB spelling).
- FadeOutAndKillEntity (q6466 / q6518): 13 rows.
- 0x00E0A510: the agent's form defined `FABLE_FORCEINLINE CTimer::operator=` in the row. At landing it became a plain `inline`
  definition plus `// Flags: /d2inlS22` with an `// Inline-budget:` line (EXTRA_RULES inline preference 2 before 3). Without the flag
  the operator is called (DIFFER 759v778); S21 differs and S22 is the smallest value that matches (`logs/inline_budget_00e0a510.txt`).
  The object still emits an unreferenced `??4CTimer` COMDAT; retail always expands the operator, so it has no owner.
- Header: engine/CCountedPointer.h (`orig` equals HEAD, 8,343 includers, none defines the macro) gets the declaration-only ShareData
  opt-in. pin_asserts pinned 0.
- Pick-any (`logs/pickany_check.txt`): `vector<CCharString>` dtor equals 0x00414EA0 (49 = 49). The ctor copy (MISSING: the owner row
  has no Leaf) grades MATCH at 0x00414DE0 with q.py; that row is the folded twin CIndexBuffer::CIndexBuffer. The other new STL COMDATs
  have no table line and no landed owner.
- Held / blocked:
  - q6386 is rejected: the PDB spelling is the global const `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A` (258 of 259
    referencing rows bind it).
  - 0x00EE2A80 is held: its port newly defines the 20-byte `??8?$CBasicString@D@@QBE_NPBD@Z` copy against owner 0x004115A0's 88 bytes.
    Copies already in landed rows are debt, not licence; an expanded-body opt-in is queued.
  - The cast-map family (q5216 / q12134 / q12135) needs whole-row ports first. q11915's self row needs NF152's header claims.
- Queued: the engine/CV_SwordInTheStoneScript.h WatchForDeathDuringSwordPull header claim, the stale g_PlayAnimationDefaultFlag /
  g_bScriptAnimationFlag table lines, and 0x00EA3490's catalog module.
- Link (`logs/link_landing.log`): 14 LNK2005 against 15 at HEAD (`logs/link_control_head.log`), the same symbol set. The second
  `??0CMainGameComponentInit@@QAE@XZ` clash leaves.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 194 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): 0x00CE1000 37 -> 2, 0x00CE1040 36 -> 1, 0x00CBE716 2 -> 1, 0x013CA828 3 -> 2, 0x00418DCA 2 -> 1.
  One grows, 0x00CBE583 0 -> 1 from 0x00D37530: SAME (an unchanged binding the table lacked). No out-of-image oracle line added.
