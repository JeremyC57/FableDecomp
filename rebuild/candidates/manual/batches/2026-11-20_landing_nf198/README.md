# Landing nf198 (Account A, 2026-11-20): remaining queue q11718 .. q12091 (20 rows)

Agent NF198 (round 10) handed back early (context) at q12017 with 50 entries: 8 done, 1 rejected, 16 blocked or partial, and 25 open
(not reached). Its files are in `naming/2026-11-20_agentNF198/`, with the held 0x0048DA62 draft (`work/held/`) and the q12058 census
(`work/shoptc_census.txt`). The open entries and seven smaller partial ports (q11724, q11902, q11920 / q11923, q11936, q11942, q12058) go
back to the queue unclaimed. The large families (q11718 / q12068 33 rows, q11982 17 rows, q11916 9 rows) stay blocked for a family pass,
not a queue slot.

- 20 rows land, all RELOCATION_MATCH, every test PASS.
- Self renames (RENAMES.tsv section 1, 9 rows):
  - 0x005ED270 is the CCountedPointer<NCameraModes::CScriptModuleBase>(T*) ctor, an explicit instance of the engine/CCountedPointer.h
    inline (the landed 0x007A4210 recipe). Its six binders already used that spelling.
  - 0x004C9C10 is `CThing::SetThingScanInclusion` (both PDBs). q11973 claimed SetAsGameStatePersisted; REJECTED.tsv has the evidence.
    Three binders follow (0x006E13E0, 0x007EECF0, 0x008ECBE0).
  - 0x00416047 `CMainGameComponent::SignalFrameUpdate`; 0x00CB0E90 is strmbase `CAMThread::GetRequest` (DirectShow BaseClasses
    wxutil.cpp, vendor rule of EXTRA_RULES 2026-11-04; a light rename only).
  - Leaf-only signature respells onto the PDB reference parameters: 0x0088C160 (the CAIBrain ctor, with an `// Inferred:` line for the
    const reference stored into the non-const member), 0x008F1CD0, 0x007D5AD0, 0x007D7520, 0x007DB280 (the tavern card game ctors take
    CThing&).
- Binders respelled in place (section 2): 0x00645CE9, 0x00833010, 0x004D5A11 / 0x004D5A44 / 0x004D5AAA, 0x0057C2E6, 0x00917DB0, and the
  port of 0x00A5C570 onto engine/CBaseClassNonCopyable.h (its local model is gone). 0x00833010 keeps its existing cast, now dereferenced.
- Catalog (q11911): 0x0122A4C0 takes the exact PDB spelling `` `dynamic atexit destructor for 'CShaderRenderManager::SingletonInstance'' ``.
  The agent asked which form to use; the coordinator chose the PDB form, the same as 0x0122AAD0. The sibling initializer 0x01223D00 and
  CTBase 0x01229260 still write the class outside the quotes; respelling them is queued.
- Rejected: q11919. 0x009C0190 is `CDisplayManager::AddDeviceResetCallback` (both PDBs); the queue's spelling is in no PDB. Its one
  stand-in binder 0x00418DCA waits for the q11916 family.
- Header: engine/CThing.h gets FABLE_RETAIL_CTHING_SETTHINGSCANINCLUSION, a declaration nested in STATEACCESS. It is add-only and
  `orig` equals HEAD. The default path is token-identical, pin_asserts pinned 0, and none of the 3,177 landed includers defines it.
- Held: the 0x0048DA62 self port (`CFontBank::GetStringWidth`) grades RELOCATION_MATCH. However, landed 0x006572A0 emits that function
  from a local inline body, so a strong owner would LNK2005. Moving 0x006572A0 onto the header is queued first.
- Pick-any (`logs/pickany_check.txt`): pickany_check is empty because every new symbol is the row's own leaf.
  - By hand: the CCountedPointer ctor copies in the five includers that define one equal the new owner 0x005ED270 (78 = 78, bytes and
    relocations). The other two references are external.
  - `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z` (header inline) has no owner row. The 0x005ED270 copy equals five landed copies, while
    landed 0x00570332 / 0x00571280 already differ in bytes. That is queued as debt; it is not a licence either way.
- Queued: the light vendor pass for 0x00CB0E30 / 0x00CB0EB0 (TU-mates of 0x00CB0E90, catalogued as CThreadMessage).
- File moves: four self rows change file name (0x005ED270, 0x004C9C10, 0x00416047, 0x00CB0E90). No funclet includes them; their paths are
  repointed in ARTIFACT_INDEX.tsv, compile-gate and manifest/functions.tsv.
- Link (`logs/link_landing.log`): 1 LNK2005, the same one as HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 60 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink (0x0057A690, 0x0048DA62, 0x004C9C10, 0x00798F50, each 2 -> 1). One grows:
  0x00462CB0 `??4CBrainUpdateZoneVector@@` from 0x0088C160, SAME (an unchanged binding the table lacked). No out-of-image oracle line.
