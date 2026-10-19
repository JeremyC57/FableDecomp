# Particle renderer cleanup, buffers, bank reset and trespass checks

00bc5420 and00bbf1c0 (217 bytes each): native lifecycle views preserve retail12-byte arrays versus donor16. Root review replaced dummy base/resource slots with real virtual destructors and PDB buffer/shader fields; target shared records remain included. RTTI identifies sprite-trail owner. Exact-source fixtures check ordered buffer/shader/array/base cleanup, null/last/shared references and trail notification. Grades4/1.

00baa6a0 (224): actual shared repeated-Z-sprite manager,128 vertices and192 indices across32 quads. Fixture checks every coordinate/index, shader selection, untouched padding, bounds and all buffer API arguments/order twice. Four grades.

009ccc00 (205): corrected propagated shader label to CBankFile::ClearEntries using bank layout and PDB evidence. Fixture tests nullable/aliased update entries, snapshot-before-clear, both tree paths, CRC vector reset and untouched state. One grade.

009580e0 (295): actual opinion/state/brain headers. Audited callee inlines both ongoing-deed and target predicates; temporaries live through each call. Tests cache timing/forced checks, missing/dead home, interface sentinel, each short-circuit result and timestamps. Three grades including shared opinion-header integration.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
