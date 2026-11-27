# 2026-11-27 landing p29: Codex pass 29 checkpoint 1 (176 of 200 rows)

Owner: "Land the landable ones and fix the ones that were marked as match but dont", then "Make sure the catalog is correct before changing to match catalog". The check that came before this landing is in `pending/2026-11-25_codex_pass29/CHECKPOINT1_CHECK.md`. `codex/` holds Codex's top-level documents; its per-row sources are now the landed rows. `logs/` and `tools/` hold the coordinator's measurements and scripts.

## Landed: 176 rows, all RELOCATION_MATCH with tests PASS (`vland.py --land`, `LANDVERIFY_STRICT_HEADERS=1`)
- **Headers:** Codex's 572 patches were rebased three-way onto HEAD (base 6834d3bfd2); 278 headers changed and 64 are new.
  - The 7 conflicts were resolved by hand (`tools/resolve.py`).
  - All 34 headers changed on both sides were checked with a conditional evaluator (`tools/ifeval.py`, `tools/checkmerge.py`): the merged text equals HEAD's under HEAD's macros and Codex's under Codex's.
  - Equivalence: 57,166 rows. The 25 landed rows whose input changed were regraded on both trees with identical grades (`logs/equivalence_differ.tsv`).
  - `engine/SalFrame.h` is not committed (see Held).
- **The five "marked as match but don't":** 0x005430F0, 0x005502E0, 0x00554740, 0x00BFAD90 and 0x00C5CA60 call shared retail-asm helpers but lacked the row's own evidence line.
  - The coordinator added `// Retail-asm:` lines citing the helper instructions at their retail addresses, read from Fable.exe:
    - `fld` + `fistp` for GFFloatToLongNear;
    - `fadd` + `fistp` for GFFloatToLongCeil;
    - FtoL's `fistp`;
    - the Pow10 `f2xm1` / `fscale` in the same TU's MillibelsToAmplitude.
  - All five now grade `retail_asm` in `crawl/purity.py`, and their bytes are unchanged.
- **Catalog names** come from the demangled leaf (`llvm-undname`), one address one name.
  - Three constructors had the calling convention in Module (`__thiscall CSubtitledMessage`). That was fixed in the catalog, manifest and file names, and the rows were regraded from the repo.
  - Two template names had demangler `class` keywords removed.
- **Catalog correctness** (owner): checked before the manifest was aligned to it.
  - 130 of the 176 selves are agree_pdb.
  - 0 landed selves differ from their address's PDB spelling.
  - The 40 split_fixable rows are correct PDB selves that older callers bind under stand-ins.
  - The GameCODA names are SDK symbols. Four (CCodaEnvironment::Commit, CSalSw2DVoice::CalcParams, CFloatReflection::Process, CFloatShortReverbModule::ProcessInput) also match the SDK body (95-100% after masking).
  - 70 manifest names were then aligned to the catalog (`logs/manifest_renames.tsv`).
- **`self_symbols.tsv`:** 176 lines added (vland does not write them).
- **`reloc_bindings.tsv`:** every new caller line is kept (406 after back-outs). Hiding a landed caller's spelling would let the tracker call its target proven while a caller disagrees, which is the accuracy rule. `logs/split_growth.tsv` and `logs/self_clash.tsv` list the lines that disagree with older spellings.
- **Reservation:** RESERVED.tsv marks the 176 as landed and their locks are released. 24 stay reserved (CODEX29).

## Backed out (built and graded, kept reserved for a later pass)
- **0x009F57A0 CInputManager::Update:** it calls 0x00A03CB0 / 0x00A03CE0 as `SetAsCharPressed` / `SetAsCharReleased`, against the registered best guesses `SetAsKeyboardCharacter` / `SetAsKeyboardCharacterRepeat`. Neither side has a source.
- **0x00C72200 CSalOpenALVoice::CalcLevelEnvelope:** it binds 0x00C595E0 as `_expf`, against the registered SDK name `?exp@@YAMM@Z`. Both are SDK symbols, so this needs settling.
- **0x00A19430 and 0x009A2000:** they declare two globals in an anonymous namespace (`ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM`, `GFontTextureCacheUpdateFlag`) that the registry has as globals under other names.
- **0x00C0B570 CRampedParam::SetTarget and 0x00C461E0 CAmbientCube::InterpolateLevel:** these are best guesses in the libpk vendor style. Registering them meets old stand-in binders at those addresses (NF453 held lines); they land with those binders' respell.
- **0x00625160 NUISystem::CUIStateDef `scalar deleting destructor'`:** it emits a 25-slot `??_7CUIStateDef@NUISystem@@6B@` where retail has 31 (r459a short-table rule).
  - The other 121 emitted table instances are at least retail's slot count (`crawl/rtti.py`).
  - 7 script classes have no retail RTTI to compare; Codex audited those slots.

## Held
- **0x00C6B260 (`_salFrameFloatToInt`, retail_asm):** waits on the owner's confirmation of the asm approval. Its new `engine/SalFrame.h` helper header fails the evidence rule (no retail address and instruction) and is not committed. The inert `FABLE_RETAIL_SAL_FRAME_FLOAT_TO_INT_PASS29` blocks in SalMath.h / CSalCpuInfo.h stay: they are plain C++ off by default.
- **Codex's 16 held rows** stay reserved: 4 DIFFER, 4 HELD_PICKANY, 6 HELD_VFTABLE, 2 HELD_SOURCE_REVIEW.
- **0x00A5C360** runs retail asm through `engine/NVectorMathSSE.h`, which carries `// Retail-asm:` evidence. `crawl/purity.py` grades it `genuine`, because it reaches the asm through an inline function; that is the documented helper-detection limit. Its grade should read retail_asm.

## Registry and tools
- **11 best guesses removed** from `descriptive_names.tsv` (`logs/REGISTRY_REMOVED.tsv`): a byte-matched Codex caller binds the address under a PDB name (9) or a GameCODA 2.10 SDK symbol (2), and a guess never stands over an original name.
  - 4 respells are in Codex's NAME_FIXES (agent NF479); the other 7 are queued (NAME_FIXES_QUEUE `p29`).
- **`descriptive_check.py` and `naming_status.py`** now compare spellings with the anonymous-namespace hash `?A0x<8 hex>@` normalised.
  - VC7.1 derives that hash per translation unit, so two rows naming one file-local symbol differ only there.
  - The tracker change alone moves 190 addresses out of false splits repo-wide: 89 → agree_unverified, 65 → agree_std, 18 → agree_pdb, 18 → agree_descriptive.

## Tracker
Proven is 35,931 of 64,151 (56.01%), or 37,949 (59.16%) with 2,018 best guesses.
- **Landing effect** (before the hash merge): 35,775 → 35,848 proven.
- **Hash merge:** a further +83.
- **116 existing addresses got worse** (68 agree_unverified → split_fixable, among others). Each one is a byte-matched Codex caller whose spelling disagrees with an older landed spelling: mostly Codex's PDB or SDK name against a stand-in or truncated leaf, plus 3 linker-folded multi-name addresses (split_pdb_multi).
  - The list and Codex's matching NAME_FIXES entries are in `logs/worklist.tsv`.
  - NF479 is applying Codex's 159 NAME_FIXES (owner: immediately after the check).
