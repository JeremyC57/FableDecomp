# Wave 2 parked-row retry, section 1 (0x00404D20 to 0x00646F67): notes

## Counts
- Worklist: 354 rows. Touched: 110 (STATUS.tsv has one line per row). 31 of them got one batch grade only: every inline
  ternary accessor rewritten as two returns, with no change anywhere.
- Matched: 14. All 14 are in src/ + test/, PASS under g++ and VC7.1 (tall.py), and the dry vland gives 14 WINS with no
  RELOC_IDENTITY. They are in payload.json and oracle_final.tsv.
- Improved: 7, in differ/.
  - 0044c6c2 matches, but only through a local def-pointer stand-in, so it is not landable yet. See "Review at landing".
  - 0053848f, 005384dd, 004b3ce0, 00625af0, 00627400: the residue is smaller.
  - 0058301a: header debt only.
- Held: 89, each with its residue and the levers tried in STATUS.tsv. Untouched: 244 rows (residues are mostly register
  colouring, scheduling or frame slots that the parked notes already show were heavily tried).
- As-is sweep: all 322 graded parked sources were still DIFFER (base/<addr>.txt, base/ndiff.tsv). 32 worklist rows had
  no grade; oracle_extra.tsv now holds their true extents.

## Per-lever wins
| Lever | Matched | Improved |
|---|---|---|
| Shared engine/CCountedPointer.h replaces the parked local CCountedPointer (declared-only members, or m_pObject spelling) | 6 (005820e7 00582673 005b358c 005b3613 005c323a 005c3444) | 3 (004b3ce0 00625af0 00627400) + 0058301a header-only |
| Over-span: grade over the true extent (BOUNDS_FIX.tsv, oracle_extra.tsv) | 2 (00405a60 0044bb00) | |
| Inline accessor statement form: two returns, not a ternary | 1 (005db5f0) | |
| Landed twin's shape (two `if` tests over inline bool accessors) | 1 (00543b10) | |
| `unsigned size() > 0` keeps the sar | 1 (005361d3) | |
| Guarded do-while, element pointer declared inside the guard | 1 (004afc60) | |
| Four-argument CRGBColour ctor from a `colour >> 8` local | 1 (00404d20) | |
| Same-TU noinline `_List_base(const allocator&)` body (tag never read) | 1 (0053cb20) | 2 (0053848f 005384dd) |
| Member objects with an inline null ctor for zeroed pointer members | | 1 (0044c6c2, matches with a local stand-in) |

Header-only migrations (residue unchanged, the parked Grade line kept): 62 sources in hdr/ moved from a local
CCountedPointer to engine/CCountedPointer.h, with members respelled (m_pObject → Data, GetPtr → Get, Clear → ReleaseData).
- They drop the header debt at no cost; land them in place of the parked sources when those rows are retried.
- Excluded because the migration made them worse: 005c66b2, 005458a0, 00554030, 005698a0.
- Not migrated:
  - 0053b91e, 005f077f, 005b6a66: they assign a CCountedPointer<CComponent> into a pointer to a derived class, which the
    shared operator= rejects; they need SetStaticCast.
  - 005bfd5e, 005148d0: other classes also have an `Object` member.
  - 004bc800, 00541fd0: no template stand-in.

## Names changed
- 00543b10: the parked NUISystem::CIconText::GetTextAlignment was invented.
  - Now `?GetAlignment@CIconText@NUISystem@@UBE?AW4ETextAlignement@2@XZ`.
  - Evidence: FableWin.pdb; vtable 0x012470CC slot 151; byte twin of the landed CText::GetAlignment 0x0054FFF0.
- 005361d3: the callee virtuals HasStateDefinition / HasStateTransition were invented.
  - Now CComponent::HasState / HasStateInChildren (slots 44/45, Ego_r `UAE_NK`).
- 005c323a / 005c3444: the local struct keys are now PDB class keys. The leaves now match FableWin.pdb:
  - `?ConstructWieldEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@@Z`
  - `...PC...PAVCComponent@4@J@Z`
  - The text bank is NGameText::CDataBank: `?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z`.
- 005c3444: the 0x005D9630 callee is `CTCInventoryItem::GetDetails` (static `__fastcall`, Ego_r `SI?AVCWideString@@J@Z`). The landed
  free `GetItemDefDescription` spelling is wrong; see NAME_FIXES.tsv.
- 0051d870 (held): the identity is `std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(unsigned int)`, not
  CRecord24Vector::resize.
- Descriptive names kept (no PDB name found):
  - g_NewLineWideString 0x013B8BA8 (005c3444).
  - g_imeCompositionAlpha_013750b0 (00404d20).
  - the stand-in malloc-node body of _List_base (0053cb20); its callee is malloc 0x00BFEA0E.
- NAME_FIXES.tsv has 8 rows of landed or bound spellings that are wrong.

## Review at landing
- 0053cb20: the same-TU `__declspec(noinline)` _List_base(const allocator&) body under auto_inline(off) is a stand-in for
  0x0053C94A.
  - Only the copy ctor is graded.
  - The binding name of 0x0053C94A (`List_Sentinel_Init` / `??0?$list@...U?$allocator`) needs the coordinator's single
    spelling; see NAME_FIXES.tsv.
- 00405a60 / 0044bb00: the catalog extents are over-span (308→208, 261→224; BOUNDS_FIX.tsv). oracle_final.tsv carries the
  trimmed bytes.
- CCountedPointer COMDAT bodies 0x004291DE / 0x00428928 / 0x00429C15 / 0x00429160 are shared across T.
  - vland warns RELOC_NAME against placeholder labels.
  - Pick one canonical spelling (NAME_FIXES.tsv).
- vland SHARED_HEADER_AVAILABLE warnings: CManager, CTCInventory, CTCInventoryClothing (005820e7 family), CQuestManager
  (004afc60), CFontBank (00404d20), CDataOutputStream (005db5f0).
  - The generated engine headers do not declare these members (for example, CTCInventory has no CreateMenuDesc), so the
    rows keep local classes.
- 005db5f0: the GetLength accessor returns `unsigned long` (two returns), while Ego_r declares `?GetLength@CCharString@@QBEJXZ`
  (long). The inline emits no symbol; with `long`, VC7.1 threads the null path again (52v56).
- 0044c6c2 (differ/): it matches once the four +0xD0..+0xDC members are pointer objects with an inline null ctor.
  - The retail ~CGameDefinitionManager 0x00450BFF shows they are CDefPointer handles.
  - engine/CDefPointer.h lacks the FableWin null ctor `??0..QAE@W4ENullDef@@@Z` (the header lists it as unproven).
  - Adding it lets this row land.
- gall.sh as copied had no QTAG. It was fixed mid-run; before that, regrades of the src/ rows wrote untagged work dirs
  `$S/wine/q/<addr>` for my 14 src addresses only.
