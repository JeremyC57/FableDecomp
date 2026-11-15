# 00DD5A60 probes

The original private virtual NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main occupies00DD5A60..00DD632A (2250 bytes). Last plain return is00DD6329 and the next allocator starts00DD6330. Both original PUBLIC/class sources prove the identity; no extent change is needed. All three complete source priors, three old tests, six neighbors, row notes/lessons/status were read in full. Best is identical to the832 prior. Historical tests only model a log; they are not reused as behavior proof. Historical declarations use local owner/vtable stand-ins that are not returned.

## Baseline and source strategies

Baseline sol2_baseline_provided regrades the unchanged historical source at2246/2250 bytes,710 built versus709 native instructions, five cleanup hunks. It is not a strategy.

1. sol2_01_original_members_locals_and_for_frame_macro: replace historical local models with original shared CEntityScriptBase/Game/Me, complete protected ParentClass reference/PMasterData and private ReleasedAndFollowing, and original TalkTimer/CellDoor/three HLStyleConversationIndex locals from module23. Ordinary real timer ctor/dtor/assignment/GetTime/operator==, CScriptThing destructor and MsgIsUsedBy are visible. Original donor uses a frame-increment for-loop with a folded false termination condition, including an unreachable post-loop thread guard. Initial for(;!false;Game.NewScriptFrame()) source gives2244 bytes/707instructions; cleanup and one assignment expansion differ.
2. sol2_02_unconditional_original_frame_increment: restore the donor's unconditional for(;;Game.NewScriptFrame()) form while retaining the dead post-loop guard. This produces2246 bytes/707instructions and only the GUARDS hit cleanup split remains. Native comment-exit timer cleanup expands; native hit-exit cleanup is called through the shared00DD5B0B block.

## Inline ladder diagnostics, not source strategies

Plain ordinary inline members were used in both source strategies. Read-only RTM observer trace is saved under sol2_02_unconditional_original_frame_increment/sol_inline_trace.txt; its observed output is asserted byte-identical to an ordinary RTM run. The two GUARDS timer destructor sites score36 (comment) and44 (hit). The required expanded timer assignment scores52. Thus no uniform threshold can leave hit44 called and assignment52 expanded. Actual per-row /d2inlT44 run sol2_02b_proved_global_budget44_conflict confirms failure at2151 bytes/664instructions. This is an owner-ladder diagnostic, not another source strategy.

Owner-authorized scoped #pragma inline_depth(0) around only the final GUARDS hit-arm thread-exit return leaves that real timer destructor call intact. Restore depth immediately afterward. sol2_02c_scoped_native_called_hit_cleanup gives exact2250/2250,709/709instructions. It does not target an allocation/register choice. No forced-inline annotation or other owner flag is used. README_CODEX explicitly permits this actual cleanup-call case after plain source and the flag diagnostic.

sol2_03_final_metadata_header_contract changes only required source headings and the new opt-in evidence comment, then regrades against the final header tree. It is not another strategy. Exact bytes and helper profiles are preserved.

## Independent proof

The only actual statically called emitted helpers are CScriptThing destructor004AA840/66 (ends tail jump to CBaseClass destructor0099A2E0) and CTimer destructor00CD4470/20. Both match complete native extents independently. Recursively audited closure totals2336 bytes. All102 code/data relocations and79 original CScriptThing vtable words bind independently; weak vector-deleting aliases follow their actual COFF weak records. Eight unreferenced shared COMDATs are explicitly listed as not landed, without native address assignment.

MakePrisonerComment00DD5860 is missing from canonical reloc_bindings. It is independently identified using both original exact PUBLICs and parent method types, four named original caller sites corresponding to RELEASE/IDLE/COURTYARD/GUARDS, and the callee's native/original conversation CFG, ConversationIndex write and identical TEXT_QST_055_PRISONER_ prefix. LOCAL_RELOC_BINDINGS.json and its proof script retain this evidence. No old catalog name is treated as proof.

The full production Main and all visible timer/destructor/MsgIsUsedBy bodies are copied verbatim into the standalone fixture. The actual shared CCountedPointer template manages the cell-door implementation, with only ABI assertions disabled in a fixture adapter.98304 full function calls (1024 configurations ×96 termination cuts) cover initial release/follow state, null door handles, door activation, hero escape, busy conversations, zero/nonzero/expired timers, comment acceptance/rejection, all hit predicate combinations, region loads before/inside a frame, every explicit termination guard, full conversation parameters/order and all cleanup. Event traces at stop n are exact prefixes of stop n+1. VC71 /Od /W3 /GR- without /EHsc and g++64 C++03 print exactly00dd5a60_TEST PASS. Distance3.0f and all integer values are exact and bounded; host long/pointer widths are retained.

Successful comments occur before their termination guards and may be left without a timer reset. A frame can load PrisonCourtyard after the loop condition while still executing that iteration's comment/hit body. A direct hit takes priority; healing-only ability hits are excluded. The final GUARDS loop is endless and exits only through thread guards. The fixture preserves these native orders.

Shared header changes add only the new independent PRISONER opt-in and original full child declarations. All three old/default preprocessing profiles agree. No old assertion moved; one child-size assertion is new. No helper debt remains. Compiler/metadata/control checks are not counted as independent source strategies.
