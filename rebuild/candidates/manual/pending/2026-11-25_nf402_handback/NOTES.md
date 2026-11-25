# NF402 hand-back (2026-11-25): the six NF388 rows held on STANDIN_LEAF

## Summary
- Batch: `batches/b1`, laid out like NF398's b1: RESULTS, SELF_SYMBOLS, TABLE_CHANGES, UNITS, rows/, tests/, orig/rows, orig/tests, head/payload.json (+ oracle.tsv), logs/, evidence/.
- **5 ACCEPT** (each row is its own unit): 0x004D2A74, 0x004D39CF, 0x004D3CDE, 0x004D4232, 0x004D5CE6.
  - Each takes NF388's literal fix plus a respell of the self to the PDB name `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ`.
  - Locks are kept as NF402.
- **1 HOLD_RULING_Q1**: 0x00C48860, plus its three EH funclet includers 0x01215810 / 18 / 20.
  - No respell was landed. RULINGS_NEEDED Q1 has one concrete proposal.
  - Locks are released.
- **TABLE_CHANGES**: one line, deleting `?Ctor@CCharString@@QAEXPBDH@Z -> 0099ebf0` (witness 004d4232).
  - 004d4232 was its only user (source census).
  - apply_table --dry: 1 delete, 0 misses, multi-target 1 -> 1, nothing new.
  - The four other respelt selfs already have their table lines, bound by CTCBase::RegisterTCs 0x004EE23F. 004d2a74 has no binder line.
- **NAME_FIXES.tsv**: 5 entries.
  - 0x004D31C0 CTCVillage::StatGetClassTypeName. It is the other definer of 4232's old stand-in, which is a MULTINAME.
  - The four GetClassTypeName callers 0x004DDA94, 0x004D8154, 0x004E07C6 and 0x004E3148. They are extern "C" stand-ins with `?Init_<addr>@T@@` callees.
- No header edits, no FLAGS change.

## Evidence per name (five StatGetClassTypeName rows)
Every row has the same 19-byte retail body, re-read from Fable.exe: `push esi; push -1; push <lit>; mov esi,ecx; call 0x0099EBF0; mov eax,esi; pop esi; ret`. This is a static `__fastcall` that returns CCharString by value, with the hidden result in ecx. The `SI` in the PDB spelling (public static `__fastcall`) is exactly this ABI.

| row | name | source 1: PDB | source 2: retail literal (read from exe) | source 3: binder / callers |
|---|---|---|---|---|
| 0x004D2A74 | CTCTargetingAI::StatGetClassTypeName | FableWin tc_register.obj | 0x01239FB0 "CTCTargetingAI" | only caller 0x004DDA98, inside CTCTargetingAI::GetClassTypeName 0x004DDA94 (no table line) |
| 0x004D39CF | CTCDCameraPoint::StatGetClassTypeName | FableWin tc_register.obj | 0x0123AEB0 "CTCDCameraPoint" | 0x004EE23F binds it (call 0x004F1797); CTCDCameraPoint::GetClassTypeName 0x004D8154 |
| 0x004D3CDE | CTCObjectAugmentations::StatGetClassTypeName | FableWin tc_register.obj | 0x0123B330 "CTCObjectAugmentations" | 0x004EE23F binds it (call 0x004F6051) |
| 0x004D4232 | CTCDHeroSuit::StatGetClassTypeName | FableWin tc_register.obj | 0x0123B9BC "CTCDHeroSuit" | 0x004EE23F binds it (call 0x004F2743); CTCDHeroSuit::GetClassTypeName 0x004E07C6 |
| 0x004D5CE6 | CTCDExplosion::StatGetClassTypeName | FableWin tc_register.obj | 0x0123D8B0 "CTCDExplosion" | 0x004EE23F binds it (call 0x004F73C2); CTCDExplosion::GetClassTypeName 0x004E3148 |

- `fablewin_pdb_names.tsv` and `fablewin_obj_functions.tsv` are one PDB and count as one source. The literal is an independent second source; the binder or caller is a third.
- The 2026-11-05 standin_s6 wave deferred these rows with "one source: catalog class only". At that time the literal was the wrong "DoCreatureAction*", so it could not confirm the class. With NF388's literal fix it now does.
- Callee: `??0CCharString@@QAE@PBDJ@Z` appears in both name sets (Ego_r and FableWin). The rows get it from `fable_string.h` under FABLE_RETAIL_CCHARSTRING, which supplies `CCharString(const char*, long length = -1)`.

## Row texts and headers
- **CTCTargetingAI, CTCDExplosion, CTCObjectAugmentations** have engine headers.
  - The static is declared on a derived class through the layout-rename include pattern, the CTCBerserk 0x004D4A86 form.
  - Preludes are the smallest that compile:
    - CTCTargetingAI: `FABLE_RETAIL_CINTELLIGENTPOINTER` + `FABLE_RETAIL_CTHING` + `engine/CThing.h`, as in the landed 0x007F6160;
    - CTCDExplosion: `FABLE_RETAIL_CCLASS` / `DEF_CLASS_BASE` / `CTCBASE` + `engine/ETCInterfaceType.h`;
    - CTCObjectAugmentations: the header's default branch, with `FABLE_RETAIL_CTCBASE`.
  - In every object, the only definitions are the leaf and its literal (evidence/object_symbols.txt).
- **CTCDCameraPoint and CTCDHeroSuit** have no engine header, so the class is declared locally.
- Stale comments in the held texts were replaced:
  - 4d5ce6 cited the literal 0x1239fb0;
  - 3cde described a `this` in ecx.
- Header debt: the opt-in branches `FABLE_RETAIL_CTCTARGETINGAI_ATTACK` and `FABLE_RETAIL_AUGMENTATION_PARTICLES` declare the static themselves, but they need the full retail macro sets. A static that touches no member does not justify that, so the default branches are used.

## Gates (logs/vland_bh.log, logs/vland_bn.log; one LANDVERIFY_WORK path work/wd_b for both sides)
- **HEAD:**
  - all 5 grade RELOCATION_MATCH PASS;
  - every row prints WARN LITERAL_DIFF, which is the control: "DoCreatureAction" / "DoCreatureActionAttack" against the retail class name;
  - every row prints WARN STANDIN_LEAF;
  - 4232 also prints WARN RELOC_NAME `?Ctor@CCharString`.
- **New:**
  - all 5 grade RELOCATION_MATCH PASS;
  - no WARN LITERAL_DIFF, STANDIN_LEAF or RELOC_NAME, and no NEQ;
  - each leaf is the full mangled PDB symbol, set from the row's `// Leaf:` line.
- **Tests:** all five are respelt and keep each row's catalog PassPattern.
  - Each is linked against the row object; the self-contained link fails by design, and verify_and_land falls back to linking the source object.
  - Each asserts the retail text, length -1, one constructor call, and construction into the hidden return slot.
  - Mutation control (evidence/test_mutation.txt): a row rebuilt with the HEAD literal links and FAILs every test, and the new rows PASS.
  - There is no test debt left. The HEAD tests of 3cde, 4232 and 5ce6 asserted nothing about the string; 5ce6's HEAD test was a bare printf.
- **WARN SHARED_HEADER_AVAILABLE** (non-blocking) prints on 2a74, 3cde and 5ce6. It is a verifier artifact:
  - `shared_header_redefinitions` runs `_strip_comments_and_strings` before `_INC_RE`, so `#include "engine/X.h"` becomes `#include ""` and no include is ever seen;
  - every layout-rename row trips it, including the landed 0x004D4A86 CTCBerserk;
  - HEAD 3cde already printed it.
  - GOTCHA proposal: match includes before stripping strings.
- **FSE overlay:** rebuild/sdk/fse_native_overlay.json lists all five as CActionDoCreatureAction::GetActionName. That is the old propagated catalog label: the real `?GetActionName@CActionDoCreatureAction@NTCScriptedControl@@UBE?AVCCharString@@XZ` is a const virtual thiscall (`ret 4`, result on the stack), while these bodies are plain-`ret` static `__fastcall` with the result in ecx and push the TC class name. refs/fse_api_manifest.json has no entry for any of the five.
- **Strict headers:** SHARED_HEADER_AVAILABLE is new against HEAD on 2a74 and 5ce6. Under LANDVERIFY_STRICT_HEADERS=1, 2a74, 3cde and 5ce6 would be rejected, as would the landed layout-rename siblings. That is the coordinator's call.
- **Uniqueness:** none of the five new symbols appears at any address in self_symbols.tsv; at file scope, only the five rows, their HEAD tests and 0x004D31C0 spell the free stand-ins (no unbound binder beyond the GetClassTypeName callers in NAME_FIXES). Each new object has only .drectve / .debug$S / .text / .rdata / .debug$F sections.
- **Duplicates:** the old free self `?StatGetClassTypeName@@YI?AVCCharString@@XZ` was a /FORCE duplicate across 2a74, 39cf and 5ce6 (NF388 link_b2.txt). After this batch no landed row defines it.
  - `?StatGetClassTypeName@@YIPAVCCharString@@PAV1@@Z` is left only on 0x004D31C0 (NAME_FIXES).

## 0x00C48860: why HOLD (RULINGS_NEEDED Q1)
- **Null search for a real name:**
  - `Mailbox` occurs 0 times in fablewin_pdb_names.tsv, egor_pdb_names.tsv and refs/fse_api_manifest.json (grep run).
  - The exe strings (`strings | grep -i mailbox`) give only `d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h`, "the queue is out of step with the mailbox handler" and "Cannot reply to the user mailbox".
  - EgoCore and the sibling repos were not checked: they are not on this Linux lane.
  - So any name is descriptive, under the 2026-11-04 vendor rule.
- **Payload identity, by evidence:** 0x00C48740 (`add ecx,14h ; jmp 0x00C167C0`) destroys the +0x14 name pointer through the same 0x00C167C0 the binder's SDecoderRequest::m_name uses, so SDecoderRequest and SDecoderMessage are one type.
- 0x00C1B030 defines the PKCMailbox<T> template inside its own row, so the shared non-template header does not block a unit; the payload and method names do.
- **Blocking conflicts:**
  - PKCMailbox exists as both a non-template class (libpk/PKCMailbox.h, used by 0x00C4BC80 and 0x00C4BE10) and a template (0x00C1xxxx rows and the table).
  - The payload has three type spellings: CMessage_00c48860 (self), SDecoderRequest (binder 0x00C47F10 and five accessor lines), and SDecoderMessage (queue rows and the bound ctor/dtor/op=; funclet 0x01215820 destroys the result through `??1SDecoderMessage@@QAE@XZ` 0x00C48740).
  - The self and the binder already disagree today: `...AUCMessage_00c48860@@XZ` against `...AUSDecoderRequest@@XZ`.
- **A right-first-time unit needs:**
  - 0x00C48860 and its 3 funclets;
  - 0x00C47F10, whose type rename touches 5 accessor lines whose selfs are `?M@C_<addr>@@` placeholders;
  - the PKCMailbox template ruling.
- The owner rules say vendor code gets light renaming and placeholder families wait.
- An interim respell that keeps the address-suffixed class is not worth landing: standin_leaf.py still flags it as an address-only name, and it would be renamed again.
- NF388's literal fix for this row stays in pending/2026-11-25_nf388_handback/held_fixes/00c48860.cpp.

## Incident (please read)
- While diagnosing a VC7.1 hang I ran `wineserver -k` once, at about 2026-11-25 01:45-01:47 UTC (just after work/cc/t2.cpp was written, mtime 01:44:53 UTC).
- The hang: a bare `#include "engine/CTCTargetingAI.h"` hits `#error` in CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE not set). Under Wine, cl.exe then never exits; it does not fail.
- The Wine prefix is shared, so that command may have killed other agents' in-flight cl.exe/link.exe processes, which would show as a TIMEOUT or SRC_FAIL in their logs around then.
- I own the mistake. Other than that I killed only my own PIDs (runpair bash, vland, cl).
- GOTCHA proposal: "A header `#error` under Wine VC7.1 hangs cl.exe instead of failing. Precheck an unfamiliar include chain with a short timeout, and never use `wineserver -k`, because the prefix is shared."

## Budget
- About 3 grade runs per row:
  - the first vland pair: HEAD completed; the new side hung on the #error include and was killed;
  - the second vland pair, complete;
  - direct prechecks.
- Plus the mutation links.
