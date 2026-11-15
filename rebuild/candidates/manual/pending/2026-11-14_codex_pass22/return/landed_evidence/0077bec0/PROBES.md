#0x0077BEC0 source strategies and diagnostics

|Tag|Source strategy or diagnostic|Result|
|---|---|---|
|01/01b|Real class/shared-header assembly; include/API gates|Compile diagnostics only, not source strategies.|
|01c|Original module locals and direct mesh temporary on shared classes|600/658; native position math stays a call.|
|02|Real static-mesh constructor visible, original params|600/658 unchanged; constructor independently177/177.|
|03|Flatten original getter forwarding to vector operator expression|600/658 unchanged.|
|04|Direct component interpolation getter|632/658; expands but wrong grouped-vector schedule. Rejected.|
|05|Named position lifetime before mesh construction|626/658; different call order/frame. Rejected.|
|06|Named displacement, scaled in place in getter|600/658 unchanged.|
|07|Explicit difference/scaled/result vector stages|600/658 unchanged.|
|08..08d|Inline-budget diagnostics with original getter/source: T100600,T1487,T500845,T200658|Seven ordinary source-shape strategies preceded these flag diagnostics. They are not distinct source strategies.|
|09|Original counted release local inside null test via existing shared RELEASE_INNER|Masked658/658; two ECX movement residues fixed. Held for audit.|
|10|Native size/frame-pointer GetSubDef specialization|Lookup162/162; main658/658. Helper flags not counted as source strategy.|
|10b..10f|Bracket smallest inline threshold:125,112,106 match;103 stays600;104 matches658|Boundary diagnostics, not new strategies.|
|11|Relocation audit corrects actual DELTA_NOTIONAL_ZERO and far subtraction5; donor double spelling|658/658 with sole qword-vs-dword FSUB mismatch; real targets now semantic evidence.|
|12|Native float5.0f subtraction|All three bodies match;21 relocations independently pass.|
|13|Exact original uid-taking base constructor and final provenance|Selected main658,constructor177,lookup162;21 relocations; all tests pass.|
|14|Final source threshold103 control, held separately under/tmp|Getter stays called at600; confirms104 is smallest. Final row source remains13.|

Nine substantive source strategies (01c,02,03,04,05,06,07,09,13), plus independent literal corrections11/12. Twenty-four compiler invocations total. Compiler diagnostics, metadata, helper flag changes and threshold probes are not inflated as distinct attempts. No parking.

Standalone complete target and real static-mesh constructor behavior test17280 combinations covers presence, existing/new persistent primitive, reason0/1/2, console draw enable, bullet time, allocation success/null, five fade inputs including exact binary fractions below/above delta, three alpha values, three interpolation values, two scales and two reference-count baselines. It checks both early exits, creation even with draw disabled, refresh forwarding, interpolated data forwarding, every constructor member, material/tint/light data, effect distances, persistence0x3000, temporary destruction and reference-count balance. Stub orientation interpolation supplies deterministic API output; external interpolation algorithm is not claimed as returned source. Valid-input invariants require a base def, nonnull appearance subtype and both interpolators; native dereferences them without defensive checks. Portable fixtures own all objects; small integral values avoid the native32/host64 long-width difference, and uid uses actual64bits. No memset-as-T initialization. Real VC71/Od/W3/GR- without/EHsc and g++64C++03 pass exact0077bec0_TEST PASS. Own runner writes SOL2_RESULTS.json only.
