# Batch `2026-10-14_linux_hand09` — hand-authored CAIStateGroup_MinionPutOutFires .. CAIStateGroup_SeekPerceivedThing functions: 40 bodies (31 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the tenth chunk of
class-labelled open functions of 17–96 bytes: building-bound state-group cleanups (the
"MinionPutOutFires::Cleanup" labels were propagated across three different classes), the
`CIEngine` and `CAIStateGroup_UsesSeat` / `StealStealableItem` constructors, four target-and-thing
records, `CActionEventExplosion(long, long, long)`, `CAIStateGroupBehaviourBase::TurnToFace` and
assorted `IsRunnable` / `Update` bodies. Callee identities fixed: 0x008FCFA0 is the brain's
thing-manager accessor (not `GetPWorld`), 0x00415FC1 is `long __stdcall GFSecondsToFrames(float)`,
0x007EE540 `IS_THING_ALIVE` is `__fastcall` on the component pointer.

Lessons recorded by the pass: `do { } while (AIAction_DoNothing() == 4)` is the loop form (a
`for(;;)` with the exit inside gets rotated); a by-value `CIntelligentPointer<const CThing>`
argument is a user copy/converting constructor built in its slot; `== 2 ? true : false` is
`sete al` while a bare `== 2` goes through ecx; the template `GetInterface` must be
`__forceinline`; a virtual base destructor in a constructor shell makes the lander pick the
scalar deleting destructor, so bases use `virtual void v00()`; a named binder with
inline-constructed filter temporaries lets the compiler drop their stores. Nine bodies differ by
filter-temporary slot order, binder store scheduling, a two-case switch shape (`sub eax,0`), or
the `& 1` mask sharing its constant with the result.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
