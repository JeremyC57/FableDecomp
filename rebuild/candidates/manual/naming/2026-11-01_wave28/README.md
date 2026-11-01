# Naming pass wave 28 (2026-11-01): 212 landed rows respelled -- ~CSpawnedFuncBase / ~CEntityScriptBase callers, CThingPhysical::InitialActivate, the CTCCombat family, CTCScriptedControl::CActionBase, CTB_Bard's ??_G

Inputs: brief `naming/briefs/BRIEF_W28.md`, QUEUE_W28_input.tsv (58 rows: wave 27 deferred / partial rows plus the c696 and c697 rows appended
mid-wave), the DECISIONS_W27_RESOLVED approvals, wave 27 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w28` (AGENT_README.md;
scripts/ here; own recheck `logs/recheck_final.log`, 212 rows, after chunks 696 / 697). No shared header edited. The coordinator copied ov_final
(212 files), applied the 4 RENAMES (git mv + build_candidates.ps1), BINDINGS (21 RETIRE / 9 ADD / 6 KEEP, 0 CONFLICT; the table equals the agent's
after-table) and LABELS (9), and rechecked the 212 rows against HEAD (`logs/coordinator_recheck.log`: 212/212 RELOCATION_MATCH + PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): d4 5 (0x00666B50 / 0x007345D0 as the PDB's const members on STLport
vector<const CAnimationEntry*>; GetComponent<CAnimComponentCombatHandedness>; PeekWorld().GetWorldSeed() -- the last junk 0x00686A80 binder
cleared), d5 3 (CActionBase nested in CTCScriptedControl; the STLport action list), d3 3 (0x004125A0 is the __false_type grow-insert, proven from the
bytes; 0x004091C0 on STLport vectors), d6 1 (0x006A5230 is ?InitialActivate@CThingPhysical@@MAEXXZ: vtable 0x01244E84 slot 9, the base call plus
AddToPhysicalList, both PDBs -- was C3DAnimationBlendState::AddCombinationBlend), clk 2 (CClickable::Update qualified; virtual CDraggable::Update),
peek 3 (CDefStringTable::StringStream PeekData; const GetStringCRC), bard 1 (0x00ED7DB0 is the compiler's
??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z), c697 3 (CEnginePrimitiveSimulationInfo::Clear; nested CLocalDetailCacheMap::CQuadTreeElement;
CChecksumFile merged into CChecksumCacheFile), sfb 150 and esb 41 (the free AutoTinyCompositeDestructor / _00f35b40 stand-ins become qualified calls
to ~CSpawnedFuncBase 0x00CDD4C0 / ~CEntityScriptBase 0x00F35B40; those 191 own rows keep Leaf lines that name no symbol -- graded on the largest
function, as at HEAD -- and are re-authored as their compiler ??_G in wave 29 per D7).

Queue outcomes (QUEUE_W28.tsv): 9 respelled, 5 partial, 10 done, 34 deferred. Decisions (`DECISIONS_W28_RESOLVED.tsv`): GetWorldSeed through
PeekWorld ratified (PDB spelling of 0x0049D810); the local CTCCombat model accepted; the declared-only explicit specialisation of
vector<CWideString>::erase ratified as an out-of-line stand-in that mangles to the bound member; 0x0040E350 held (one folded copy, three element
types); the CSpawnedFuncBase header family is wave 29 scope 1; LoadFileList deferred (in neither PDB); the 191 sfb / esb own rows re-authored as
their ??_G in wave 29. Not reached: scope 3, the ~CScriptThing (~66 rows) and ~CTCBase (21 rows) binder families, most c697 destructor families.
