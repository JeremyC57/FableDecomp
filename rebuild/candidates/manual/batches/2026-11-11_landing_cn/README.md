# 2026-11-11 landing cn -- naming agent NF19: 130 landed rows respelled to proven callee names, 6 rows renamed

Source: `naming/2026-11-11_agentNF19/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). NF19 worked the
name-fix queue (99 entries: 63 done, 34 partial, 2 rejected).

**Verification**
- vland --recheck-landed ran in two parts: all 130 grade MATCH or RELOCATION_MATCH.
- Running every test exe found its catalog PassPattern in 130 of 130. vland's FAIL column on 0x004107E0, 0x004110B0, 0x006C7370 and
  0x00C0C910 comes from their custom PassPatterns.
- Bindings were re-derived (-260 / +198 lines). bindings_multi_check is at its NullCoord baseline.

**Respells (callers bound to a corrected spelling)**
- GetDef 30, RemoveThing 14, Transfer<T> 13.
- The land1011cd STLport binders: 33 (std::__destroy_aux, __uninitialized_copy / _fill_n, __copy).
- CTCBase FrameUpdate / OnInitialActivate 8, tavern NewGame / NewRound / OnInitialActivate 5, and the rest single binders.

**Renames (RENAMES_applied.tsv)**
- Proven by PDB or caller:
  - 0x007AEEC0 CTCBoss::CountLivingHelpers
  - 0x0072D360 CCreatureActionBase::SetPItemRemoved
  - 0x007F1B70 CTCDRumble::Create
  - 0x009E4AA0 std::list<CMaintainedEvent>::push_back
- 0x00C0C910 CString::MakeUpper is descriptive: libpk has no PDB, and the body is CopyBeforeWrite plus _strupr.
- 0x0086E5C0 is a catalog-name fix only.
- **Not applied:** 0x00C35450 `CEnvelope_012b37dc::Stop`. Address-suffixed class names are not allowed, so the row keeps its old name.
- The first apply put the class into the name column (`CTCBoss::CTCBoss::...`). It was reverted and redone with module/name split
  before the recheck.

**Headers**

Five `#ifdef` opt-ins were added, with default=same and each header a line-superset of the repo copy:
- CString.h: FABLE_RETAIL_CSTRING_MAKEUPPER
- CTCPhysicsBase.h: _GETMASS and _DUMMYRHSET
- NUISystem_CMouseCursor.h: FABLE_RETAIL_CMOUSECURSOR_GETPOSITION
- CCreatureActionBase.h: FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
- stlport/fable_containers.h: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL

**Queued (NAME_FIXES_QUEUE, land1011cn)**
- 0x00AF0D80: FAM02's rename to __destroy_aux<CCountedPointer<CGuiVarTransferBase>*> is wrong. The element is
  CCountedPointer<NParticleEngine::CParticleSystem>, per callers 0x00AF0EF0 and 0x00AF10E0.

**Left (APPLIED.tsv gives the reasons)**
- Stand-in classes with no shared header: about 45 binders.
- Folded template addresses: the FAM05 resize family and 0x00AB34C0.
- About 90 namespaced GetPItem stand-ins were not started.

Labels: ghidra_out/labels_landing_2026-11-11cn.tsv.
