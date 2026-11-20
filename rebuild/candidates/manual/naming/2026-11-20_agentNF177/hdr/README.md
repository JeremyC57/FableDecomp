# NF177 header overlay

All edits are `#ifdef` opt-ins with member-specific macro names. With none of the macros defined, every header here is token-identical to
the repo copy it was taken from (base copies in ../orig/), except for one pin_asserts.py respelling in
candidates/landscape_background_lod_compiled.h (`FABLE_STATIC_ASSERT(...)` -> `FABLE_STATIC_ASSERT_AT(57, ...)`, which keeps the same typedef
name fable_static_assert_57).

## fable_retail_banks.h (base: rebuild/include/fable_retail_banks.h, working tree 2026-11-20)
Added inside `class CBankFileManager`, public section, right after the FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS block:
- `FABLE_RETAIL_CBANKFILEMANAGER_BANKFILEPATHS`: declarations `CWideString GetDevBankFilePath(const CCharString&) const` (0x009A7CA0) and
  `CWideString GetRetailBankFilePath(const CCharString&)` (0x009A7D40). Ego_r.pdb + FableWin.pdb spellings (QBE / QAE).
- `FABLE_RETAIL_CBANKFILEMANAGER_BANKEXISTS`: declaration `bool BankExists(const CCharString&) const` (0x009A80B0). Ego_r.pdb + FableWin.pdb.
- `FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS`: the inline `CCharString LookupBankAlias(const CCharString&) const` (Ego_r.pdb
  ?LookupBankAlias@CBankFileManager@@QBE?AVCCharString@@ABV2@@Z; no Fable.exe copy). Needs FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP. Body as
  the landed 0x009A7F80 row's local member.
- `FABLE_RETAIL_CBANKFILEMANAGER_FINDCONTAINEDBANK`: declaration of the landed 0x009A7F80 FindContainedBank (Ego_r.pdb, both outputs by
  reference), same public section.
- `FABLE_RETAIL_NBANKFILEMANAGER_CRETAILBANK_GETTHREADEDFILE`: inside `NBankFileManager::CRetailBank`, after FilePath: the inline
  `CCountedPointer<CThreadedFile> GetThreadedFile()` (FableWin.pdb QAE; no Fable.exe copy). Needs the CThreadedFile type.
Used by rows 0x009A7CA0, 0x009A7E20, 0x009A7ED0, 0x009A80B0 (first three / BANKEXISTS / LOOKUPBANKALIAS) and 0x009D56C0 (the last two).

pin_asserts.py: pinned 0 (the header's asserts are FABLE_STATIC_ASSERT_AT with explicit line numbers).
Landed includers (`grep -rlE 'fable_retail_banks.h' rebuild/src/compiled`): 20 rows. None defines any of the five NF177 macros, so the
opt-ins change none of them. Other agents' overlays: NF174 / NF175 / NF176 overlay no copy of this header (checked at start).

## engine/CBankFile.h (base: rebuild/include/engine/CBankFile.h, working tree 2026-11-20)
- `FABLE_RETAIL_CBANKFILE_GETBANKPATHNAME`: in the FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS branch, right after the `public:` that follows
  the virtual block: declaration `CWideString GetBankPathname() const;` (Ego_r.pdb 0x00B19C30 + FableWin.pdb 0x02F918E0, retail 0x009CBF10).
  Used by rows 0x009D56C0 / 0x009D5820.
pin_asserts.py: pinned 0 (FABLE_STATIC_ASSERT_AT with explicit line numbers).
Landed includers (`grep -rlE 'engine/CBankFile.h' rebuild/src/compiled`): 116 rows. None defines the new macro, so the opt-in changes none of
them. No other agent's overlay holds this header.

## engine/CTCWeapon.h (base: rebuild/include/engine/CTCWeapon.h, working tree 2026-11-20)
- `FABLE_RETAIL_CTCWEAPON_ISUSABLEBY`: in the GENERATED (non-retail) branch, after the FABLE_RETAIL_CTCWEAPON_AUTOLOADING block: declaration
  `bool IsUsableBy(const CThing&) const;` (Ego_r.pdb 0x0063A950 / FableWin.pdb 0x0239AF10, retail 0x005DAB10; the retail branch already
  declares it unconditionally), plus a `class CThing;` forward declaration before `#pragma pack(push, 1)` under the same macro.
  Used by rows 0x005DAD10 / 0x005DAD60.
pin_asserts.py: pinned 0 (FABLE_STATIC_ASSERT_AT with explicit line numbers).
Includers (`grep -rl '"engine/CTCWeapon.h"' rebuild/src rebuild/tests rebuild/include`): 70. None defines the new macro. Older overlays of this
header (agentFAM10-13, NF101, NF113, NF25b, NF26, NF27) are earlier rounds whose opt-ins are already in the base.

## engine/CEngineStaticMapFileBlockManager.h (base: rebuild/include/engine/CEngineStaticMapFileBlockManager.h, working tree 2026-11-20)
- `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ISBLOCKLOADED`: public section, right before the SETBANKFILEENTRY block: declaration
  `bool IsBlockLoaded(long block) const;` (Ego_r.pdb 0x00A41DE0 / FableWin.pdb 0x02DF7220, retail 0x00BE0480). Used by row 0x00BE0480.
pin_asserts.py: pinned 0. Includers: 6. None defines the new macro. NF172's overlay copy is an earlier round (its opt-ins are in the base).

## candidates/landscape_background_lod_compiled.h (base: rebuild/include/candidates/landscape_background_lod_compiled.h, 2026-11-20)
- `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ISBLOCKLOADED_CONST`: in the local `class CEngineStaticMapFileBlockManager`, the
  `bool IsBlockLoaded(long blockId);` line becomes `#ifdef ... bool IsBlockLoaded(long blockId) const; #else <unchanged> #endif`.
  Used by row 0x00BDC180 and its test (rows/00bdc180_test.cpp defines the macro and a const stub).
pin_asserts.py: pinned 1 (the sizeof(CMemoryDataInputStream) assert, old line 57, now FABLE_STATIC_ASSERT_AT(57, ...): same typedef name). Includers: 2 (row 0x00BDC180 and its test).
NF131's overlay copy is an earlier round (its GETBLOCKDATA opt-in is in the base).
