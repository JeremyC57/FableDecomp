# NF224 notes (2026-11-22, round 10): the nf223 follow-ups (q12559-q12561)

Base: the repo working tree with nf223 applied. Locks: 0x00494840 0x0054ED90 0x0042DB20 (were `released`) and 0x00415B80 (new mkdir), owner NF224
(rows.txt). No AUTH11 lock or Codex pass 27 reservation touched (AUTH11 has no header overlay; UNMATCHED_ROWS.tsv has no line for any of the four addresses). Tools: NF223's, re-pathed (tools/).

## Entry 1 (q12559): 0x00494840 / 0x0054ED90 GetFont binders -- blocked, coordinator decision
- Retail expands every CCountedPointer member at both sites. Disassembly:
  - 0x00494840: the only calls are CCharString, GetFont 0x009E2C80, the count block's destroy, operator delete 0x00BFE9BC and ~CCharString.
    The converting operator=, ShareData, ReleaseData and the temporary's release are all in place.
  - 0x0054ED90: the same, with ??4 / ShareData / ReleaseData <CFontBank> in place.
- So every existing decl-only opt-in puts a call where retail has none. Forms (NF223's drafts + one `#define`; work/e1/grades.txt):

  | row | CONVERTING_ASSIGN / ASSIGN_DECL_ONLY | SHAREDATA_DECL_ONLY | RELEASEDATA_DECL_ONLY |
  |---|---|---|---|
  | 0x00494840 | DIFFER 118 v 187 | DIFFER 124 v 187 | DIFFER 111 v 187 |
  | 0x0054ED90 | DIFFER 304 v 365 | DIFFER 310 v 365 | DIFFER 290 v 365 |

  RELEASEDATA_NOINLINE also keeps the release out of line (a call), so it cannot match either; not graded.
- Only the default (all inline) is RELOCATION_MATCH. That /O2 TU then emits /O2 COMDAT copies, and the owners are /O1 bodies from the
  0x0042Fxxx / 0x00419xxx size TUs (`and [esi],0`, out-of-line calls). Copies with a retail owner (pk.py, work/e1/pk_forms.tsv):
  - 0x00494840, 3 NEQ at HEAD -> 3 in the draft. HEAD: ??1 / ReleaseData / ShareData <const CFontBank> v 0x0042FC0E / 0x0042F948 / 0x0042F9F2.
    Draft: ReleaseData and ShareData <const CFontBank> stay, ??1<const CFontBank> goes, and ??$?4<CFontBank> 72 B v 0x0042FE34 (18 B) arrives.
  - 0x0054ED90, 0 -> 3: ReleaseData<CFontBank> 53 v 0x004057A0, ShareData<CFontBank> 67 v 0x004190E2, ??4<CFontBank> 72 v 0x00419463.
    At HEAD these were NUISystem::CCountedPointer<NUISystem::CFontBank> (NO-OWNER).
  - ??1<CFontBank> (NO-OWNER in both drafts): no retail out-of-line copy. callers.py on 0x004057A0 finds only E8 calls, no E9 thunk.
- The same compiler emits these COMDATs from a TU that expands them, so retail's own InitGraphics / GetDataFromDef TUs very likely did
  too; the linker kept the size-TU copies.
- Decision needed:
  - (a) accept the copies as nf203-class debt (both drafts are ready as they are: NF223 work/e2/, copies in work/e1/); or
  - (b) authorise a size-body opt-in probe (the nf213 `_SIZE_BODY` pattern in stlport/fable_containers.h). That would be new opt-ins on
    CCountedPointer.h. Not tried: outside "existing opt-ins", and 3 forms are spent.
  - Until then 0x009E2C80 stays at 3 spellings.

## Entry 2 (q12560): 0x0042DB20 ~CManager on engine/NUISystem_CManager.h -- blocked, coordinator decision
- Leaf ??_GCManager@NUISystem@@IAEPAXI@Z: FableWin.pdb (raw grep and ghidra_out/fablewin_pdb_names.tsv) and Ego_d.pdb (raw grep). Neither a
  raw grep of Ego_r.pdb nor egor_pdb_names.tsv found it (repack4k.py not run), so the two sources are FableWin + Ego_d. The callee is ??1CManager@NUISystem@@IAE@XZ (0x0041E641, all three PDBs).
- In the header, ~CManager is protected and non-virtual (`I` in both manglings), so the ??_G is not in the vtable. Only a delete-expression
  or an explicit dtor call with access emits it, and no public inline member of the header does one.
- Forms (work/e2/):
  - f1: a delete-expression on a CManager* in an anonymous-namespace derived class (0x009A0AE0's shape) fails with `error C2248:
    'NUISystem::CManager::CManager' : cannot access protected member declared in class 'NUISystem::CManager'` (work/e2/q_f1.txt). Reading
    it as the protected-access-through-a-base-pointer rule is inferred: the message names CManager, not ~CManager.
  - f2: `this->~CManager()` in the derived member fails with `error C2300: ... class does not have a destructor called '~CManager'`
    (work/e2/q_f2.txt).
  - NF223's three are already known: a made-up member, a qualified dtor call (no ??_G), and the friend (rejected, nf95b).
  - f3 (probe overlay work/e2/hdr_probe, NOT handed back): add-only opt-in making the real PDB member Clear inline
    (`delete GPUniqueInstance; GPUniqueInstance = 0;`) plus the PDB static, called from an anonymous scaffold.
    - The leaf comes out RELOCATION_MATCH, binding ??1CManager@NUISystem@@IAE@XZ at 0x0041E641.
    - But the TU also emits a COMDAT ?Clear@CManager@NUISystem@@SIXXZ. It is 39 B with bytes equal to 0x0042FB40, and it clashes with
      0x0042FB40's strong definition (lnk.py: LNK4006 "Clear already defined").
    - Its static mangles `0` (private) where all three PDBs say `1` (protected static), which is fixable.
    - It adds a 3rd spelling of 0x013B8710.
- Options for the coordinator:
  - (a) A 3-row unit:
    - the header opt-in (inline Clear + protected static GPUniqueInstance, PDB names; the inline-ness rests on one weak source: 0x0042FB40
      sits among the TU's template COMDATs 0x0042F864..0x0042FE34);
    - 0x0042FB40 ported onto the header (it is itself a HEAD-local model of NUISystem::CManager), its leaf the Clear COMDAT through a
      scaffold, so COMDAT meets COMDAT;
    - 0x0041E5F2 respelled g_pUIManagerInstance -> GPUniqueInstance.
    - Then 0x013B8710 goes 2 -> 1 (absrefs: only 0x0041E5F2 and 0x0042FB40 reference it) and 0x0041E641 2 -> 1.
  - (b) allow the friend form (NF223's draft).
  - (c) leave the row as at HEAD.
- The row is unchanged and 0x0041E641 stays at 2 spellings. NAME_FIXES.tsv records GPUniqueInstance for 0x013B8710.

## Entry 3 (q12561): 0x00415B80 -> CGraphicDataBankInit::CGraphicDataBankInit -- done
- Not catalog-only. The row is landed (stand-in row-local struct SNavigationLayerDefaults_00415b80, its own ??0 as the leaf); NF223's
  census said it was not landed (REJECTED.tsv).
- Names: ??0CGraphicDataBankInit@@QAE@XZ in Ego_r.pdb, FableWin.pdb and Ego_d.pdb. SNavigationLayerDefaults is in no PDB.
- Callers: callers.py gives 4 E8 callers (0x00416C8A 0x0041E3F6 0x0042DDB3 0x005BFF07), all landed and already binding that spelling
  (nf223). absrefs: none. 0x00415B80 goes 2 spellings -> 1.
- Row: engine/CGraphicDataBankInit.h default path (no opt-in), and a placement-new scaffold in an anonymous namespace emits the header's
  inline ctor COMDAT. This is the 0x00BB1740 / 0x00485420 pattern (descriptive internal name ReferenceGraphicDataBankInitConstructor).
  - 0x00BACE10 (CEngineSubPrimitiveSprite ctor) is the constructor-leaf precedent: a header inline ctor emitted as the row's pick-any
    COMDAT through anonymous-namespace scaffolding.
  - One form, MATCH (48 B).
  - Defined symbols v HEAD: the leaf replaces ??0SNavigationLayerDefaults_00415b80. Added: ??0CPixelFormat (NO-OWNER header inline),
    ??2 placement new (EQ v 0x00405C10) and the hashed scaffold.
  - 0x00B2CB40's /O2 copy of the same ctor is EQ v 0x00415B80 (pk.py).
- Test: rewritten on the shared header (HEAD's was a host model of the class). FormatIndex is protected, so it reads the seven dwords
  from storage.
- Recheck (recheck_final.log): 5 rows = 0x00415B80 MATCH plus the 4 unchanged binders RELOCATION_MATCH, all PASS.
  - The STANDIN_LEAF and RELOC_NAME warnings are the catalog name, which the coordinator renames (RENAMES section 1).
- Link (work/link_check.txt): 0x00415B80 + the 4 binders + 0x00B2CB40, 0 duplicates. Each object defines ??0CGraphicDataBankInit as a COMDAT.
- Definer census: `grep -rl CGraphicDataBankInit rebuild/src/compiled` = 15 files. None defines the ctor out of line (no
  `CGraphicDataBankInit::CGraphicDataBankInit`). Only 0x009F83D0's local model has an in-class inline ctor, and its COMDAT is EQ v
  0x00415B80 (work/e3/pk_f1.tsv). So there is no strong / COMDAT clash.
- Binders: recheck_final.log RELOC_NAME lines show all 4 callers on ??0CGraphicDataBankInit@@QAE@XZ -> 00415b80.
- Labels / manifest lines carrying the stand-in or the chunk label: LABELS.tsv. Table: none (TABLE_CHANGES.tsv).
- Includers: no source includes the row file.

## Self-check
- rows/: 00415b80.cpp + _test.cpp only, and recheck_final.log covers it. Entries 1 and 2 change no row (drafts and probes in work/).
- Census: 0x00415B80 2 -> 1. 0x009E2C80 (3), 0x0041E641 (2) and 0x013B8710 (2) are unchanged. Nothing grows.
- hdr/: no header opt-in handed back.
- Flags lines: none. EH funclets: none.
