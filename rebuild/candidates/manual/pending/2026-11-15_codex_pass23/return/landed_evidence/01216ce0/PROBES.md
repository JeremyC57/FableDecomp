# Pass23 attempts

1. sdk_body_01: original DirectShow Disconnect/DisconnectInternal source and shared complete SDK models. Parent127/127 masked bytes exact on first real RTM build; emitted full graph also exact (cleanup8, handler10, CAutoLock10, metadata36). No source-shape workaround needed.
2. main_generated_review_01: reserved wrapper includes true parent, recompiled independently. All four reachable sections remain exact; all12 relocation edges and four external contracts verified. Compiler-local cleanup is $L1270, not baseline $L1269 (the include changes compiler numbering). Wrapper Leaf is the actual emitted parent; both generated entry offsets separately checked.

Native required test native_sdk_behavior_01:340 connection/result/replacement cases, /Od /W3 without /EHsc. Supplemental native_sdk_eh_01 and GCC64 C++03 each add two throwing-callee cases to verify automatic unlock and connection lifetime. All PASS.

The supplied parent CA9710 Release is wrong. Exact handler-pointer trace reaches CA97A0 Disconnect. No failed source experiment or attempt count was hidden. One genuine authoring family plus one wrapper validation, already exact; ten failed attempts are not applicable to a matched row.
