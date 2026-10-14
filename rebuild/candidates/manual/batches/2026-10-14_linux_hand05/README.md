# Batch `2026-10-14_linux_hand05` — hand-authored CAC_HighScorePost .. CAIStateGroupBehaviourBase functions: 40 bodies (35 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the sixth chunk of
class-labelled open functions of 17–96 bytes: creature-action and script constructor shells,
`CAIBrain` state-group lookups (`FindNextStateGroupToUse`, `IsUpdateZoneActiveThisFrame`),
`CAIStateGroupBase::Initialise` / `CleanupMain` / `SetPushableByHero`, `CAIFilter` evaluations,
`CAIGameCameraBase::PeekCurrentPos`, `CAFile::GetFileInformation`, a `CDynamicObjectHeap`
allocator, sound-pair array helpers and named-handle constructors. Three EH-framed constructors
were already landed by the eh passes; eight rows with rejected propagated labels landed
`_unattributed` / `_global` with descriptive leaves. 0x00C628F0's chunk row over-captured a second
fragment after the padding; the 19-byte body is what landed.

Lessons recorded by the pass: in a constructor shell a vtable store before register-zeroed stores
and before the argument store is an embedded member object with an inline constructor plus the
argument assigned in the body (init-list members sink the vtable store); stores entirely before the
vtable store are an inlined `__declspec(novtable)` intermediate base constructor; frame-pointer
bodies want `#pragma optimize("y", off)` + `("s", on)` only; plain-store schedule follows statement
order; an inline bitset test must return the raw masked `unsigned long`; an inline accessor on an
intermediate object swaps the eax/edx allocation against a direct member chain; `if (grow) Refill();
else return 0;` hoists the failure return. Two `CAIBrain` bodies differ by the materialised
group-name address in a `CCharString` compare loop.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
