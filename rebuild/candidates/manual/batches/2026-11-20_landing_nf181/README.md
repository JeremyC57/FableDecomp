# Landing nf181 (Account A, 2026-11-20): ctor / dtor names, slice 1 (classes C0..CH), 78 rows

Agent NF181 (round 10) took the ctor / dtor entries for classes C0..CH: 32 entries, 4 done, 4 done-already, 3 rejected, 4 blocked
(the TU-emitter rule, and authoring for q11949), 17 open (16 of them census-complete with rows left listed, several partly ported). Its
files are in `naming/2026-11-20_agentNF181/`; `work/` holds the compile-probe census of every REFS row (`census.tsv`, `probe_*.tsv`)
and the defined-symbol check script. The open entries go back to the queue unclaimed.

- 78 rows, all RELOCATION_MATCH, every test PASS.
  - 61 rows: the protected ??0CBaseClassNonCopyable@@IAE@XZ (q11902). Each row's local model now declares the ctor protected; access only.
    0x00686800 / 0x0080C250's standalone tests got the same in-place access fix.
  - Self renames (4): CCreatureAction_BeetleLand ctor 0x00982C70 and CCreatureActionBase ctor 0x00693C70 move to the reference forms
    (q11740 / q11676), with their binders 0x009211D0 and 0x0069F230 / 0x00842460 / 0x00843F50 / 0x00854610 / 0x00854660 / 0x008546C0 /
    0x00857F30. The ??_G rows 0x009568E0 (CCreatureAction_ThrowShotObject, q11897) and 0x00485420 (CMemoryDataInputStream) use the nf86
    scaffolding over the shared headers' inline ctors.
  - Other respells: 0x00951B20 (~CCreatureAction_DragonTakeOffIntoFlying, virtual) and 0x0095B280 (~CCombatActionBase, virtual).
    0x008A8E40 moves onto engine/CCombatAction_GenericResponseToDecapitation.h. 0x0064B6DA takes const CFontBank& (q4907). 0x00485CF1
    moves onto engine/CPersistContextReadSection.h and binds ~CDataStream at 0x00993B10.
- No header changes (hdr/ empty). Catalog: 0x009B08C0's HEAD `// Flags: /O2 /Oy /W3` line is now copied into its catalog CompilerFlags
  (set_catalog_flags.py).
- Pick-any (advisor consulted, owner delegated). NF181 asked whether 0x00485CF1 / 0x00485420 may land. Their objects newly define
  `??1CDataInputStream` / `??1CMemoryDataInputStream` (5 bytes) and `??1CPersistContext` (1 byte). Binding-table lines attribute these
  to 0x00993B10 / 0x009BBBF0, which pickany_check reports as DIFF (`logs/pickany_check.txt`). Those lines come from other rows' stand-in
  binders, not ownership:
  - 0x00993B10 stores CDataStream's own vtable 0x0129A69C (`mov [ecx],0x0129A69C ; jmp 0x0099A300`). Its row Leaf is ??1CDataStream (Ego_r),
    and both rows bind exactly that Leaf there (gate pairs). The short copies are reached at no retail site.
  - 0x009BBBF0 is CPersistContext::CheckUnknownSymbols (Ego_r, landed row). The ??1CPersistContext line comes from 0x009AE150's binding.
  - So no landed row owns these copies (the 0x004473A0 ??1CSavedInputEvent precedent), and both rows land. HEAD includers 0x00597228,
    0x0047F58B, 0x004A21F0 and 0x00A4FE60 define the same copies. The wrong labels and table lines are queued.
  - Byte-identical copies:
    - 0x00485420's ??0CMemoryDataInputStream equals the owner 0x00485400 (25 = 25 bytes, OK). Its ??_7 is the shared header's,
      identical to the HEAD includers (stand-in DATA COMDAT rule: the owner's 4-byte stand-in vtable differed at HEAD).
    - 0x009568E0's ThrowShotObject ctor (87), vtable (280) and ??_G (30) equal the owner 0x009566C0's HEAD object byte for byte,
      relocations equal (checked by hand; no table line, so pickany_check could not see them).
- Link (`logs/link_landing.log`): 13 LNK2005 against 12 at HEAD (`logs/link_control_head.log`). The symbol sets differ by exactly
  ??0CMemoryDataInputStream@@QAE@XZ: 0x00485400 defines it strongly out of line, and 0x00485420 now emits the identical header copy.
  Five HEAD objects (0x0047F58B, 0x004A21F0, 0x00520670, 0x00597228, 0x006F9F90) do the same, so it is queued as debt on 0x00485400.
- Recheck (`logs/recheck.log`): 78 RELOCATION_MATCH. rebind multi-check 1 (baseline); split gate 0 created. Referencing-row recheck
  (`logs/includer_recheck.txt`): 54 good, 4 bad. The bad four are untouched July landscape rows at their HEAD grades: 0x00BDC510
  1044v1131, 0x00BE7D70 1437v2983, 0x00BE8920 328v365, 0x00BF3B60 825v1480. They are bad with the same numbers in nf108 through nf152 and
  good in none. Owner recheck good.
- Oracle fix: rebind wrote `??_7CNavBlockedQuadTreeNode@@6B@ -> f8db543c` for 0x00A768C0, a target outside the image. Retail
  `mov [esi],0x0129DD3C` sits at the object's dir32 offset 0x2B, and the oracle bytes there are `3cdd2901`. The HEAD line `0129dd3c` was
  restored by hand. The older out-of-image line for 0x012165B8 is queued.
- tablecheck (`logs/tablecheck.txt`): six addresses shrink (0x0099A300 30 -> 29, 0x0099A2F0 3 -> 2, 0x00693C70, 0x00543910, 0x0066A560,
  ...). Three grow by a spelling their row already bound at HEAD (SAME): 0x01238C6C ??_7CThingPointer (0x00693C70), and 0x00994700 /
  0x00994780 the CFileDataInputStream ctor / dtor (0x009B08C0). Those bindings were simply missing from the table. The fourth GREW line
  was the 0x00A768C0 slip above.
