# agentNF4 notes: binder re-author onto the CIntelligentPointer / CParentDefClassBase headers

## Result
68 rows were re-authored in `rows/`, covering 69 binder lines (009207a0 binds two targets). Every row grades
RELOCATION_MATCH in the final regrade (`work/results.tsv`: bad=0, hard=0). There are 22 new `_test.cpp` files,
and each prints PASS on g++ -std=c++11 (64-bit) and on VC7.1 /Od. The other 46 rows still pass their landed
tests unchanged (`work/landedtests.tsv`, 46/46 PASS on both compilers).

| Target | Done | Left |
|---|---|---|
| 00a01c10 ??1CBaseIntelligentPointer@@UAE@XZ | 27 | 63 |
| 00a01b10 ??0CBaseIntelligentPointer@@QAE@XZ | 5 | 28 |
| 00430370 ??0CParentDefClassBase@@QAE@XZ | 26 | 0 |
| 00430300 ??1CParentDefClassBase@@UAE@XZ | 11 | 1 |

## Left, and why
Every unattempted row is listed in STATUS.tsv with its reason. In summary:
- **Action dtors whose class is ambiguous** (13 rows): 007eaf00 007eb990 007ed420 007edff0 007ee7a0 0085edf0 00922eb0 00935970 00966890 0097d280 009400f0 00968150 00968170.
  - Several CCreatureAction_* classes share one layout, and no ctor, Clone or ??_G pins the dtor to a single class.
  - 00968150 and 00968170 could be PickUpJug or PickUpThingAtTable.
  - Guessing would break the one-address-one-name rule.
- **State groups** (10): these need retail CThingCreatureBase / CThingObject chains as header opt-ins.
  - 009575c0 needs a retail CThing that has a CBaseObject base.
  - 0096f180 had a register-order diff with the by-value T* ctor. Neither a /d2inlS budget nor a residue fixed it.
- **std_Destval stand-ins** 008fd2ca 008fd544 008fd769: the owning container element was not proven.
- **Camera-mode / AI temporaries** (10): these need NCameraModes and AI headers with retail members.
- **STLport algorithm instantiations over CIntelligentPointer<T>** (13): the instantiation owner and element headers were not pinned.
- **Rows on invented CRetailTree / CAggregate stand-ins** (9): the real class is not identified.
- **00454b4c**: proven to be ~CThingBaseDef. The CThingComponentSet member at +0x39 has dtor 0x00450BA0. The row still needs a CThingBaseDef.h retail opt-in, which I ran out of time to write.
- **11 rows whose identity is not yet proven from two sources** (004dcd2b 004e1437 00646709 006592e2 0065f0fa 006c1be0 006e1e60 007004b0 00713510 007699c0 00787e12).

## Decisions worth checking
- **009207a0** carries `// Flags: /d2inlS20`.
  - This is an inline-budget residue (owner rule 2026-11-08). It is the smallest budget that expands the CIntelligentPointer(T*) ctor inline; S15–S19 do not.
  - IsRunnable is now `protected` per Ego_r, so the self symbol is `MAE`, not `UAE` (see NAME_FIXES).
- **_Construct rows** (004aa030 00511f50 0074cc90 00a53520) use the STLport spelling `_Construct<T,T>` (`V1@`).
  - Both PDBs are Dinkumware builds (`<T,const T&>`). Retail links STLport 4.6, so the PDB spelling cannot be the retail one.
  - The catalog names `_Cons_val` / `CIntelligentPointer::CIntelligentPointer` become `std::_Construct`.
- **Map/multimap ctor COMDATs** (0x00453465 0x004dfa89 0x0045433f) are /Os-shaped. The rows that bind them use `#pragma optimize("s",on)`, the same as 004d6931 and 0065e3a3.
- **Def classes keep implicit dtors.**
  - Declaring `virtual ~X()` in a model stops ??_G from inlining the dtor.
  - The generator and the overlays declare no dtor.
- **Local models** are written only where no `engine/<X>.h` exists: CSkyDef etc., CTCCrateStack, CTCDiceGame, ShowTrophy, the NCameraModes chain, CSimVoice, and CObjectFamilyEntry.
  - Classes that only have scalar members are spelled with their Ego_r members, not padding.
  - Promoting them to shared headers is follow-up work.
- **Descriptive names**: none were introduced. Every new name is a PDB spelling.

## Header debt (not fixed)
- The default path of `CEnvironmentDef.h` derives from CDefClassBase. Retail RTTI says CParentDefClassBase. The overlay opt-in carries the retail form.
- `CCardPositionsDef.h` declares `virtual ~`. Retail def dtors are implicit.
