# Batch `2026-10-14_linux_hand10` — hand-authored CAIStateGroup_SellToBuyer .. CASoundBank functions: 40 bodies (36 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the eleventh chunk of
class-labelled open functions of 17–96 bytes: summoner action constructors (`#pragma
optimize("s", on)` for the `and dword,0` initialisers), `CEngineInternalAttachedPrimitiveInfo`
and a four-vtable primitive-type-info constructor, scalar deleting destructors of `CGameMap`,
`C3DMeshFileUniqueVerticesChunk`, `CTCAnimationComplex` and `CAInputProcess`, a `novtable`
`CTCBase`-derived destructor, `CRumbleEffect::Initialise` (frame-pointer forwarder returning
`*this`) and state-group `IsRunnable` / `OnActivate` bodies. `SleepOnFloor::OnActivate`
reproduces a retail bug (ignored `GetInterface` result, uninitialised out pointer on the miss
path) and documents it in the source.

Lessons recorded by the pass: `test al,al ; mov ecx,esi ; je ; call ; mov ecx,esi ; Terminate`
is the early-return form `if (!ok) { Terminate(); return; } Execute(); Terminate();`; a
counted-pointer release needs the pointee in a named local; a `??_G` whose body contains calls
needs `__forceinline` on the virtual destructor; `mov eax,1 / xor eax,eax` from a bool function
is an int `&&` chain; constructor-temporary arguments evaluate the object after the pushes;
`C2DVector` two-dword copies keep retail's store order only through a user memberwise copy
constructor. Three bodies differ by an address materialisation (`add eax,12Ch ; mov eax,[eax]`)
or the load order of a position copy.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
