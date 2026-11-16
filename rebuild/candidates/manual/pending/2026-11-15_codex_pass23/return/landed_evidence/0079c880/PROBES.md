# 0079c880 probes

Prior hand299/hand648/Q6 sources and handback notes read. Their local fake GetSubDef/CalcAndLookup bodies were discarded. Historical attempts moved effect/def declarations and scopes with those fake bodies; no such body remains.

| Attempt | Lever | Grade |
|---|---|---|
| sol_baseline_localmodel | Diagnostic exact emitted public/local model | DIFFER933v933,6strict hunks,loose1.0 |
| sol_port01 | Genuine shared types/callees; original virtual leaf | compile fail, RemoveTC requires existing state-access opt-in |
| sol_port02_remove_tc | Proper shared GetSubDef/GetEffectFromString bodies and original calls/fields | DIFFER933v933,6strict frame hunks,loose1.0 |
| sol_shape03_implicit_emitter | SLOWTIME call assigned directly with implicit pointer-to-CIntelligentPointer conversion; END keeps original named pemitter | MASKED_MATCH933v933,267instructions,0hunks |
| sol_final01 | Existing assert pins preserved | MASKED_MATCH933v933,267instructions,0hunks |
| sol_final02_ctor_fixture | Original constructor declaration under new feature for normal-lifetime fixture | MASKED_MATCH933v933,267instructions,0hunks; full59-operand audit |

The original FableWin local table has one pemitter, inside END's scope, and none in SLOWTIME's scope. That is the evidence for the direct first assignment. Its extra artificial named pointer had changed the frame-packer tie: the direct form puts pcreature at retail's slot and the converting temporary in its own8-byte slot.

Native sol_native01: VC7.1RTM/Od/W3 WITHOUT EHsc compile/link/runPASS. GCC64 C++03 PASS against the same immutable final b1b611...1bcf5 include tree.14behavior scenarios exercise pause/decrement/recreation/dead or missing emitters/subdef and lookup failure/expiry/removal/dead or live target/zero or nonzero fade. Exact event string DSMDENRCG asserts the two effect phases, removal and blur updates in order. String/refcount assertions prove temporary cleanup.
