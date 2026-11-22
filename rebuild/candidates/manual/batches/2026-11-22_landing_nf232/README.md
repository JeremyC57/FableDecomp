# Landing nf232 (Account A, 2026-11-22): the nf231 follow-ups (three self rows, one extra binder, the two CText::SetValue row ports)

Agent NF232. Hand-back: `pending/2026-11-22_nf232_handback/`. Entries q12629-q12632 (q12633 blocked, q12634 not started). No header change.

- **Recheck** (`logs/recheck.log`): 6 rows, all RELOCATION_MATCH + PASS, no RELOC_CONFLICT, no warnings; HEAD control the same. Link: no
  duplicates (`logs/link_check.txt`).
- **0x004336B0** self row now `?Set@CStartupDefinitionManager@@SIXPAV1@@Z` on the landed SET + PCLASS / CCountedPointer RESET_CAMERA_DECL
  opt-ins (body `PClass.Reset(manager)`); 2 -> 1 spelling. Files renamed (`global_operatornew_004336b0` -> `CStartupDefinitionManager_Set_004336b0`),
  catalog CEngineSubPrimitiveBase::operator_new -> CStartupDefinitionManager::Set. Callee 0x00433D3A's Pool stand-in -> the FableWin
  `Reset<CStartupDefinitionManager>` spelling (stays 2: its own self row is a stand-in, queued). Data 0x013B8760 3 -> 2 (`PClass`).
- **0x00411B00** stand-in `vector_push_back_copy` -> the STLport `vector<CDateAndTime>::erase` (explicit instantiation, landed COPY_NOINLINE);
  the HEAD stand-in's size pragma is gone (TU default /O2, 16-aligned entry). 2 -> 1. `__copy` callee 0x0040E350 onto the spelling
  0x0059AEAF emits (copy EQ the owner). PassPattern unchanged (a test token).
- **0x00412450** CRetailVector stand-in -> STLport `vector<CWideString>::erase` (3 -> 2; the last stand-in binder 0x00495780 is held: an
  in-place respell would define begin / end / clear under names the header defines inline, so its header port is queued).
  **0x00412B70** (extra binder) -> STLport `resize` plus two callees; copies EQ their owners.
- **0x0054F410 / 0x0054F370** NUISystem::CText::SetValue: ported onto the shared headers on the first form, leaves unchanged; every local
  model gone (incl. the 98-placeholder CText). 0x0129AAF4 binds the table's `?NullChar@CCharString@@1DB` instead of a literal. Pick-any
  GetCRC / GetLength / GetAsCharArray copies EQ (HEAD's GetCRC copy was NEQ: relocations). New header tests (4 cases each).
- **Tests:** new header tests for 0x004336B0, 0x00411B00, 0x0054F410, 0x0054F370.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 4 stand-in lines deleted, 1 added (`Reset<CStartupDefinitionManager>` at 0x00433D3A).
- **Pick-any** (`logs/pickany_vs_retail.tsv`): every owned copy EQ.
- **Coordinator rulings on NF232's questions:**
  1. The CNewFrontendGameComponent vtable / ctor / dtor family: approved as the next family item (one header unit, the 4-row family), with
     the inline `~CGameComponent` measured at stock flags first; no FORCEINLINE exception (ASM_RULES allows `__forceinline` only where
     retail used asm, never for a compiler residue). The 0x01230C94 table line moves with it.
  2. 0x00495780: header port queued.
  3. 0x00411B00's PassPattern: kept (a test token, not a symbol).
  4. Tests for 0x00412450 / 0x00412B70 and the 0x00433D3A self row: queued.
- **Queued** (tag land1122nf232): NF232's NAME_FIXES, the vtable family, 0x00495780, 7 rows on HEAD host-model tests.
