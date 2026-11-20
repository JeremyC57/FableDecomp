# Landing nf187 (Account A, 2026-11-20): game / creature / UI / input member names (16 rows)

Agent NF187 (round 10) handed back early (context) with 67 entries: 8 done, 16 done-already, 8 blocked, 1 deferred and 34 open (not
reached). Its files are in `naming/2026-11-20_agentNF187/`. The open entries, the partial q6321 / q6456 and q6396 go back to the queue
unclaimed. q6396's binder was locked by NF185, which has since landed.

- 16 rows land: 2 MATCH + 14 RELOCATION_MATCH, every test PASS.
- Self renames (3):
  - 0x01228E44 becomes the compiler's `_$E2` of NUISystem::CList::GPreviousSelected on engine/NUISystem_CList.h, under the static-pair
    `optimize("s")` shape of nf89 / nf164.
  - 0x00615920 moves onto CPersistContext_TransferableVector.h / CPersistTraits.h / CTCInventoryMap.h, with its test rewritten on the
    headers.
  - 0x00A04030 is respelled SetAsJoystickPosition1 -> SetAsJoystickPosition (both PDBs; its only caller already binds that name).
- The static pair's initializer 0x0121A6AC (`_$E1`) grades RELOCATION_MATCH from the same source, so the pair lands together. Its local
  NUISystem::CList / STLport models are gone and its test is rewritten on the header.
- The other rows respell callees onto PDB names: ~CWorld (UAE), the CInputEventPackage ctor by reference, CTCSummonSpell's private static
  `?CurrentCreatureGlobalDefNum@CTCSummonSpell@@0JA` (3 of 4 rows), the FableWin `~list<NUISystem::CList*>`, and others (STATUS.tsv).
- Header: stlport/fable_containers.h (`orig` equals HEAD) gets FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY, a new `#elif` branch. The default
  `~list() {}` would emit a 5-byte copy that differs from 0x0053C9BC's 23 bytes, so the pair rows declare ~list only. The default path is
  token-identical, pin_asserts pinned 0, and none of the 1,819 includers defines the macro (all rechecked good).
- Coordinator calls:
  - q4154 access: the pair defines GPreviousSelected under the header's default public 2V spelling. The landed initializer 0x0121A6AC
    already defined that spelling at HEAD, and 16 binders use it, so the address gains no spelling. Option (a) is accepted. The queued
    nf179 header claim on engine/NUISystem_CList.h (protected 1V and the FableWin accessor) moves the pair with the binders.
  - The pair's LNK2005 on GPreviousSelected is the static initializer / atexit-destructor pair rule (EXTRA_RULES 2026-11-20), queued as
    debt.
  - q6323: the claimed `SummonCreatureGlobalDefNum` is rejected for the PDB private static. 0x0072AB40 needs an access decision in
    engine/CTCSummonSpell.h (friend or other): header claim queued.
  - 0x008CCD70 and 0x007DE5D0 define a row-local `class CTCParticleAttacher : public CTCBase`. The class has no shared header, the
    landed siblings 0x007DE710 / 0x00772BB0 use the same shape, and the base is engine/CTCBase.h, so this is accepted. 0x007DE5D0 keeps
    one C-style downcast of GetTC(TCI_PARTICLE_ATTACHER), cast debt under nf105. A shared engine/CTCParticleAttacher.h is queued.
- Pick-any (`logs/pickany_check.txt`):
  - `_List_base<CList*>::clear` equals its owner 0x004855A3 in both pair rows (39 = 39).
  - The agent graded 0x00615920's CUIMiniMapMarker ctor copy RELOCATION_MATCH against its owner 0x005FA530.
  - The agent compared every other new COMDAT with the table and with every Leaf line: none has an owner.
- Link (`logs/link_landing.log`): 6 LNK2005 against 5 at HEAD (`logs/link_control_head.log`). The extra one is
  `?GPreviousSelected@CList@NUISystem@@2V...`: the pair rule above.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 1854 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): three addresses shrink (0x00A475C0 3 -> 1, 0x007F8860, 0x00662D80). Five lines grow:
  - 0x004A6AB0 `??1CWorld@@UAE@XZ` and 0x009F5690 `??0CInputEventPackage@@QAE@AAVCAFile@@@Z`: callers move onto the self row's own
    Leaf (narrowing).
  - 0x009F6A70 / 0x009F6FE0: SAME. 0x009F5D70 bound them at HEAD; the table lacked the lines.
  - 0x0053C9BC: the stand-in `Invoke_0053c9bc` gives way to the FableWin spelling. The count is unchanged; the self row's placeholder
    Leaf port is queued.
