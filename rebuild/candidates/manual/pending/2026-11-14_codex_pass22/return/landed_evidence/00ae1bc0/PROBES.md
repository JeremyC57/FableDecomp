# 00ae1bc0 source strategies

All two prior drafts, ROW.md and all row handback files were read before authoring. The supplied Q24 source is the actual global CParticleSimulation member with reference parameters. Baseline reproduces the historical residue. Current trials contain 10 distinct source families; repeated diagnostic spellings within a family, baseline restores, portability repair and debugger runs do not count toward that floor. Header variants are guarded by new opt-ins, old/default branches retained and shifted assertions pinned. All experimental headers were pruned from the final row.

| Attempt | Family | Change | Size / strict hunks |
|---|---:|---|---|
| sol00_supplied_baseline | — | Baseline or restored final verification; not a new strategy. | 1158 / 1 |
| sol01_typed_float_reads | 1 | Use the declared ReadType<float> accessor directly for all float reads. | 1158 / 1 |
| sol02_typed_gravity | 1 | Only the gravity accessor; diagnostic within family1. | 1158 / 1 |
| sol03_qualified_position | 2 | Existing qualified GetPosition accessor in the inline Read position guard. | 1158 / 1 |
| sol04_named_position | 3 | Materialize unsigned position before the range check. | 1158 / 1 |
| sol05_position_then_add | 3 | Stage the addition into the named position; diagnostic within family3. | 1158 / 1 |
| sol06_named_force_setters | 4 | Original PDB-named SetGravityStrength/SetWindStrength field setters with explicit inline bodies. | 1158 / 1 |
| sol07_nested_positive_read | 5 | Nest the Read body under positive size and valid-position guards. | 1158 / 1 |
| sol08_slow_first_read | 6 | Put chunk overflow first with an early return, then the fast memcpy arm. | 1160 / 31 |
| sol09_chunk_seek_accessor | 7 | After memcpy use the existing real SetPositionAssumeWithinSourceChunk accessor to update all stream fields. | 1057 / 68 |
| sol10_real_overflow_body | 8 | Make the landed real overflow helper body visible/noinline; its internal fields use shared declarations. | 1158 / 1 |
| sol11_duplicate_gravity_tail | 9 | Duplicate the gravity read between the positive alpha-byte and zero-alpha arms. | 1254 / 8 |
| sol12_duplicate_force_tail | 9 | Extend the duplicated branch to both gravity and wind reads; diagnostic within family9. | 1345 / 9 |
| sol13_positive_allocate | 10 | Put the whole success region under positive AllocateArrays with a single false return after the region. | 1164 / 3 |
| sol14_final_best | — | Baseline or restored final verification; not a new strategy. | 1158 / 1 |
| sol15_main_union | — | Baseline or restored final verification; not a new strategy. | 1158 / 1 |

No family improved the best residue. Baseline and final best are 1158 bytes, 439 instructions, strict ratio 0.9954441913439636, loose ratio 0.9977220956719818. Final regrade against main merged_headers_01 is unchanged. Retail 00AE1DD4 uses mov eax,[ebx+4]; lea ecx,[eax+4]; ours uses mov ecx,[ebx+4]; add ecx,4. This remains DIFFER, not an accepted match. Source and overlay hashes for every immutable trial are in ATTEMPTS.json; local objects and disassembly remain in history/00ae1bc0.

The old named-alpha-count/Read-directly/member GetParticleCount/sizeof ordering/gravity local/Size guard/fake ReadArray and /Gr /Ow /G6 probes were read as historical failures and not re-counted. /Oa historically fixes the pair but breaks the frame; it was not used.

Compiler diagnostics: adapted the existing Pass20 c2prio/c2dbg tracer to current row, a copied c2.dll, and unique executable names. Five bounded attachment runs did not yield a usable trace. GDB16/Wine11 first rejected conflicting target-feature responses; disabling that packet and selecting i386 then timed out attaching/connecting. The first serial-reaper test briefly blocked the compiler queue; only its unique debuggee was stopped, and later runs used an authorized row-owned concurrent subreaper with 180-second child limits. All row-owned debuggees and the row-owned server were cleaned up. The shared compiler/prefix/server were untouched. Scripts and failure logs are retained locally under diag; there is no successful trace evidence to infer allocation state from.

Main retry: inspected all17existing grade receipts and fullsource-family ledger. Remaining firstgravity load chooses ECX/add instead of native EAX/LEA. The archived stream_probe contains an untried const-reference position strategy (prior compiled only value locals). Test that real field reference under the actual full caller, preserving existing/default Read code behind a new row opt-in. Baseline now accepted header30. This is a load-timing diagnostic, not a claim of another complete source family.

Main02 const-reference Read position worsens1190bytes/111hunks; remove experimentalheader. Main03 fresh original donor inspection finds a real missed accessor:03332C68 calls ReadULONG, not draft ReadSLONG, for TimestepChanged. Raw module1151 scope names the arguments is and alloc, with no locals. Restore those exact names and unsigned accessor; full325-byte donor target and Read helper tail recorded privately. Native Read helper update order already agrees with current original source; do not permute it without evidence.

Main03 correct original unsigned flag reader/argument names leaves1158/onepair. Main04 restores the actual visible STLport copy template body using the existing shared nontrivial-pointer __copy_ptrs/__copy implementation. Previous work only made overflow visible; both original copy calls remained declaration-only. Retailcopy00AE61E0 is57bytes and copies allfour vector words. This is real library code, not a callee stand-in; test plain template visibility first, and independently grade any reachable emitted copy.

Main04 plain visible copy moves target1175bytes: wrapper is expanded while the loop helper stays called, unlike native calls to copy itself. Owner-ladder diagnostic main05 uses smallest speed inline budget1; existing required Read accessors already have established forced-inline devices. If internal copy helper expansion is also lost, test scoped depth only at the two native-called copy sites next.

Main06 scoped call boundary restores1158-byte target with unchanged gravity pair, but instantiates copy wrapper with inner expansion suppressed (23byte wrapper, rather than native57byte whole copy). Main07 materializes the genuine template instantiation before the scoped call site to test whether VC71 captures the nested budget at instantiation. Native copy call sites are AE1D56 and AE1D6A; the main06 source comment mistyped those sites, corrected in07. No target match claimed.

Main07 explicit instantiation still emits23byte copy wrapper; no changed root pair. Main08 increases actual speed inline budget to200 while maintaining the native call-site depth boundary, to allow original nested loop helper expansion inside the wrapper. This is solely helper-inline closure; reject if target expands/changes incorrectly.

Main08 budget200 retains the same1158byte root/two-instruction gravity residue. The visible-copy series did not improve the root; restore main03 correct original parameter names and ReadULONG instead of retaining unnecessary inline devices or extra unmatched emitted helper code. All snapshots preserved. Next test direct original low-level read signature scope, guided by PDB/donor rather than expression grids.

Main10 new donor-guided source family: original Read has outer no_bytes>0 scope and an early return ONLY for invalid overflow_check; earlier sol07 instead nested both positive guards. Fresh PDB module548 gives exact dst/no_bytes/overflow_check names and types. Restore those with actual donor guard/update sequence, guarded shared-header branch, original assertion identities retained. main09 correct-root fixture rerun on VC71 and GCC03 PASS before this attempt.

Main10 RESULT: exact1158bytes/439instructions,16exact calls, no reached helper debt. Restoring original outerpositive/no_bytes scope and keeping invalid-check earlyreturn repairs the single gravity mov/lea pair. Fresh full root+actual Readbody fixture passes VC71/GR- noEH andGCC03:9end-to-end scenarios plus46 Read-boundary cases (zero/negative lengths, range reject,32bitwrap). Both original rawPDB classes/names re-extracted. Header old/default140profiles:135success,5identical preexisting prerequisites. Main audit receipt review/main_00ae1bc0. No new inline control or calling convention used in selected source.

Causal control main11: keep the recovered dst/no_bytes/overflow_check names and every expression, but restore the old top-level no_bytes<=0 earlyreturn. This isolates original outer positive lexical/control-flow scope from identifier corrections; matching header restored immediately after compilation. The control is diagnostic only, never selected.
