# Batch `2026-10-14_linux_hand12` — hand-authored CActiveFile .. CAnimFilterAnyGroup functions: 40 bodies (34 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the thirteenth chunk of
class-labelled open functions of 17–96 bytes: animation-component allocators, binary transfers,
`DiscardDeviceIndependentResources` and `UnloadResource` bodies (double null store of a
`CIVCountedPointer` assignment), `IsInWorkHours` instances, the `CTesselationData` and
`CAnimationSet` constructors, `CAnimationEntry` / `pair<unsigned long, CAnimationEntry>`
assignments, blend-in and rotation queries, combination-animation lookups and a Jenkins name
hash. Many manifest labels were propagated and were corrected from the RTTI vtables and PDB
layouts (retail `CVectorMap` is a 12-byte vector plus two bytes; `CAnimateThing` offsets sit 0x28
below the PDB).

Lessons recorded by the pass: a `long` that feeds `fimul` must share its frame slot with a dead
out-pointer local through an inline getter; a virtual call on a known global object is
devirtualised unless made through the member pointer; `if (!a) { if (b) { if (!c) return 0; } }
return 1;` gives the three-tests-to-one-tail result; a forwarding `Add()` needs `__forceinline`
or the lander grades the jump thunk; a method that stores an argument and calls a method on it
is not a constructor. Six bodies differ by cross-jumped zeroes, a counted-pointer release
shape, a dead store the DDK eliminates, swapped temporary homes or tail scheduling.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
