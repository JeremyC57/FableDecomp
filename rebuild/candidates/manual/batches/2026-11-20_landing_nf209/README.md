# Landing nf209 (Account A, 2026-11-20): held drafts 0x008E9850 / 0x009A0AE0, ScytheInfoScript ??_G, SetCurrentGroupName binders (4 rows)

Agent NF209 was the single running agent (owner order: one agent at a time). It handed back (context) at the q11772 / q4389 boundary:
4 entries done (q11692, q12295, q11879, q4860), 3 blocked or returned (q11760 / q11873, q11772) and 1 not reached (q4389, the
FP-COMBATMAP leftovers). Its files are in `naming/2026-11-20_agentNF209/`, with the held 0x0047DB73 draft (`work/held/`).

- 4 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x008E9850: NF205's held draft, unchanged. It was checked under the advisor ruling (EXTRA_RULES nf205 amendment).
    - SetCurrentGroupName equals the owner 0x0073DFF0, 77 = 77 bytes, relocations included.
    - The GetCRC copy equals 0x00404310, 46 = 46. Its one differing relocation is the owner's stand-in `g_EmptyString` for the same
      retail address 0x0129AAF4.
    - q.py of the row source grades RELOCATION_MATCH at 0x00403A10 and MATCH at 0x00403A00 (coordinator re-run).
  - 0x008D9530: the last q11692 binder, ported onto engine/CAnimFilterOfGroup.h. All 8 referrers of 0x0073DFF0 now bind
    `?SetCurrentGroupName@CAnimFilterOfGroup@@`, and the SetGroup / SetAndHash split closes (0x0073DFF0 3 -> 1 spellings).
  - 0x009A0AE0: self rename to `??_GCMemoryAllocator@@IAEPAXI@Z` (FableWin.pdb + body, nf86 scaffolding). The advisor ruling's three
    conditions hold:
    - The LNK2005 set is unchanged.
    - 0x0122A9A0 binds neither symbol.
    - The old self leaf is bound nowhere (table 0, sources 0).
    - 0x0122A9A0 / 0x009F2B20's differing byproduct copies are queued as debt.
  - 0x00E2AB30: self rename to `??_GCQS_ScytheInfoScript@NScript@@UAEPAXI@Z` (nf86 scaffolding, on engine/CLevelScriptBase.h). Its old self
    leaf `?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z` is in no table line. The many CThingSwitch_* files that spell the same
    stand-in name each define it for their own address; none binds 0x00E2AB30. CQS_ScytheInfoScript has no header, and the landed CQS_*
    siblings use the same local shape.
- Split gate (`logs/split_gate.txt`): it reports 1 created at 0x00CBD510, `??1CScriptBase@@UAE@XZ` from 0x00E2AB30. That spelling is already
  bound at 0x00CBD510 by landed 0x00DCC000 (table line, and its source declares the virtual ~CScriptBase). The gate scanned only the rows left
  after the universal-binder exclusion (168 referencers), so this narrows the address and adds no spelling. The 148-byte body is
  CScriptBase::~CScriptBase (catalog, engine/CScriptBase.h). Respelling the self row's stand-in `??1CRecordSet_00cbd510` and 0x00D80C00's
  CLevelScriptBase binding is queued.
- Pick-any (`logs/pickany_check.txt`): the OK lines equal their owners: 0x00404310, 0x0073DFF0 and 0x009AC2D0.
  - The coordinator graded the MISSING lines with q.py, the row source at each owner address: 0x00405C10 placement new MATCH, 0x00403A10
    RELOCATION_MATCH, 0x00403A00 MATCH.
  - The 0x00CBD510 MISSING line is the 5-byte `~CLevelScriptBase` inline. 0x00D80C00's stand-in binder maps it there; it is not that
    address's body, so it does not make 0x00D80C00 the owner (nf181 addition).
  - The vtables have no owner object.
- Link (`logs/link_landing.log`): 1 LNK2005, the same as HEAD (`logs/link_control_head.log`). 0x00E2AB30's inline ctor copy is byte-identical
  (113 = 113) to the strong definition in 0x00E299F0. They clash only when both objects link, as in the agent's
  `logs/agent_q4860_link_new.log`. Coordinator re-run: q.py of the row source at 0x00E299F0 grades RELOCATION_MATCH, and cmpbody gives
  113 = 113 with equal bytes and relocations. That is accepted as debt (nf181 / nf184), and porting 0x00E299F0 is queued.
- Held (advisor rule, pick-any): 0x0047DB73 (q11772). Its RELOCATION_MATCH draft newly defines an 8-byte
  `?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z` COMDAT. The address 0x004118C0 is the retail out-of-line HasTC, which landed 0x00488620
  defines as 58 bytes. It needs a declaration-only HasTC opt-in first. Coordinator check: the HEAD source of 0x0047DB73 never calls HasTC (it used a local struct), so
  the copy is new, not HEAD debt (nf203 does not apply). Advisor decision (owner delegated): the hold stands.
- Returned: q11760 / q11873. The 0x006D41F0 self row reaches DIFFER 307v295 after 3 forms (an inline split around 0x004115A0); /d2inlT 61..64
  is untried. q4389 was not reached.
- Queued: 0x004118C0 HasTC (catalog row missing), 0x00CBD4E0 `?OnPersist@CScriptBase@@` (FableWin + the vtable slots).
- Table: the stale `?FreeThis@CEngineSubPrimitiveGameMesh` line at 0x009F1F90 is gone (0x009F1F90 2 -> 1). Also shrinking: 0x0099EC30
  61 -> 60, 0x00CDBFB0 4 -> 3, 0x0073DFF0 3 -> 1. None grows, and no out-of-image oracle line was added.
- Recheck (`logs/recheck.log`): 134 good, 0 bad.
