# Naming pass wave 25 (2026-11-01): 75 landed rows respelled -- CPixelFormat::Initialise, CEntityScriptBase::IsActiveThreadTerminating, OakValeRevisited, CSimpleMemoryPool, CTCBase::PeekWorld binders, the implicit-dtor rows

Inputs: brief `naming/briefs/BRIEF_W25.md`, QUEUE_W25_input.tsv (41 rows: wave 24 open rows, the 18 c692 rows, four DECISIONS_W24 items), wave 24
DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w25` (AGENT_README.md; scripts/ here; own recheck `logs/recheck_final.log` 76
rows incl. 0x009F31C0, which it withdrew after the recheck -- the 75 kept files are md5-identical to the rechecked ones). The coordinator copied
ov_final (75 files), applied the 12 RENAMES (git mv + build_candidates.ps1), BINDINGS (46 RETIRE / 3 ADD / 6 KEEP, 0 CONFLICT; the table equals the
agent's after-table) and LABELS (13), and rechecked the 75 rows against HEAD (`logs/coordinator_recheck.log`: 75/75 RELOCATION_MATCH + PASS).
`bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): oak 2 (the NScript::CCS_OakValeRevisitedScript ctor 0x00EE8100 and ??_G 0x00EE8C10), pkc 1
(PKCAutoDelete<...>::GetChecked 0x00C09920), cfl 1 (~CChunkedFileLoader 0x00AA7540), smp 5 (private ClearMemoryBlocks; ~CSimpleMemoryPool 0x00987F10;
the `_$E2` atexit destructors of MatrixBufferPool / VectorBufferPool / GroupVectorBufferPool 0x0122AB10/30/50), ibuf 4 (CIndexBuffer::Uninitialise --
was Release -- and CVertexBufferWin32::Uninitialise; ~CEnginePrimitiveManagerParticleLineGroup 0x00BC5420), esb 7 (IsActiveThreadTerminating on an
empty global CEntityScriptBase -- was IsDone), peek 5 (the PeekWorld binders, PeekThingSearchTools, SetFaction, OnHeroWalkedThroughDoor), coh 3 (PDB
order: 0x00681940 GetToleranceToBeingHit, 0x00681980 GetToleranceToBeingHitRemaining -- was CViewBase::CalculateDisplayDistance -- 0x006819D0 OnHit),
bcnc8 8 (the implicit-dtor rows on the protected virtual ~CBaseClassNonCopyable), pixfmt 39 (CPixelFormat::Initialise(_D3DFORMAT) with the inline ctor,
six stand-in classes folded into CPixelFormat, ConvertRGBToPFDData, GetLevelByteLength; 19 rows gained a Leaf line).

Queue outcomes (QUEUE_W25.tsv): 9 respelled, 3 partial, 8 done, 6 hold, 16 deferred. Decisions: `DECISIONS_W25_RESOLVED.tsv` -- 0x0139C8FC is
`_NullString`: retail's own assert text in Fable.exe reads "stringSrc.GetData() != _NullString" and no _afx name occurs in the binary (coordinator
search), reversing the wave 24 MFC choice; 0x0139C900 / 0x0143BB2C held (no retail name exists); 0x005136B0 held (two retail copies); 0x00C2D9C0 owner
deferred (no two sources). Deferred (DEFERRED.tsv) -> wave 26 with the c693 queue rows; scopes 2 and 3 were not reached.

Housekeeping: the agent created two stray directories outside the scratchpad (/pw, /ov_final: compile scratch and an overlay copy from unset shell
variables); the coordinator did not delete them on the agent's request and left them for the owner.
