# Landing 2026-11-06j: chunk 809 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows (sharing the lane with the chunk 808, then 810, agents). The six rows were unnamed gap-scan entity
Mains (5.5-6.0 KB). The agent proved every name from slot 1 of the entity vtable the landed Alloc / ctor stores and both PDB name
sets. The landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00DA0EF0 | NScript::CQ_MinionCampScript::CBriarRose::Main | RELOCATION_MATCH PASS (3 attempts; `DoChant` `__forceinline`: in both PDB name sets but with no retail copy; a named ONHIT `CCharString`; the chant-bar colours built right to left) |
| 0x00F063B0 | NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Main | RELOCATION_MATCH PASS (2 attempts; a same-file `inline __declspec(noinline)` `CBasicString<char>::Compare` body, chunk 807's lever) |
| 0x00F0D630 | NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::Main | RELOCATION_MATCH PASS (2 attempts; `for (; !T; NSF)` and a row-local `inline CScriptThing::CScriptThing() {}`) |

- **Headers** (`hdr_README.md`): the default paths are textually unchanged.
  - engine/CQ_AmbushTradersScript.h: FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY and `_CONVOYSPY` (the nested classes need `_MAIN`).
  - engine/CQ_NewOakValeIntroScript.h: FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_GUARD (its fifth entity opt-in).
  - New engine/CQ_MinionCampScript.h (0xAC) and engine/CV_IntroductionToTrophiesScript.h (0x6C), behind `#ifdef FABLE_RETAIL_NSCRIPT`
    with no `#error` branch. Debt (agent_NOTES.md): opaque `std::list` words in CQ_MinionCampScript.
  - The agent rechecked all 8 landed includers of the two edited headers with the overlay: RELOCATION_MATCH PASS (SIBLINGS.tsv).
- **vland warnings:** RELOC_ALIAS on the `rand` thunk; RELOC_NAME on 0x00CDBFB0 (the queued map-dtor placeholder) and on 0x00415D70
  (labelled `CDefString::operator CCharString`, the demangled form of the bound `??BCDefString@@QBE?AVCCharString@@XZ`: same name).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06j.tsv`, 16 rows: the six Mains, three unlanded helpers (0x00D9E8B0
    CBriarRose::Alloc, 0x00F05A10 RefreshSpyAllegiences, 0x00F0EE90 WatchForHeroProximity) and seven vtables. apply_labels respelled
    the manifest's 0x00F05A10 with its NScript scope.
  - The agent's other 33 label rows are landed functions whose catalog names already agree (or differ only by the catalog's omitted
    `NScript::` prefix), so they are not relabelled. The two Guard data globals 0x013AC840 / 0x013AC844 carry descriptive names
    (`g_NOVIGuard...`; no PDB name) used only by a parked row, so they are left out of the labels file.
  - 5 queue lines (tag `land0611j`): 0x00DA2610 DoSpeech bound without its class nesting by the landed 0x00DA27B0 (the new row adds
    the PDB spelling, so the target carries two symbols until that row is respelled); 0x00F05A10 bound without NScript by 0x00F058D0;
    0x00DAC580 bound as `CGuard::Alloc` (it is CNOVI_Guard) by 0x00DABAC0; 0x01375748 bound under three names, none in either PDB
    (owner pick); 0x00CDBFB0 (known).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand809/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DAC760 NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main | DIFFER(5906v5867), 18 attempts | one cleanup pad where VC7.1 expands the implicit seh_me (STB) dtor once more than retail; RELOCATION_MATCH under the diagnostic `/d2inlT72` (not a landing option) |
| 0x00E8EF80 NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main | DIFFER(6038v5996), 13 attempts | the same STB dtor split; also matches under `/d2inlT72` |
| 0x00E9E420 NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main | DIFFER(6035v5915), 11 attempts | retail calls ~CTimer / ~CScriptThing / the movie dtor at the shared cleanup pad where ours expands one more of each; the in-loop IsAlive called in retail; ebp zero vs pushed immediates (chunk 807's 0x00EE57B0 family) |

The differ sources carry no `/d2` flag; their tests are `differ/<addr>.test.cpp` (copied from the agent's test/).

**Owner question (with chunk 804's fans):** the two STB rows match only with `/d2inlT72`, a VC7.1 inline-threshold diagnostic flag.
Nothing shows retail was built with it; they stay parked until a plain C++ spelling or an owner ruling.
