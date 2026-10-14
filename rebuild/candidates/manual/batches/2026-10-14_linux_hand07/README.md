# Batch `2026-10-14_linux_hand07` — hand-authored CAIStateGroup_FireflyWander .. CAIStateGroup_KidPlayHopscotch functions: 40 bodies (37 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the eighth chunk of
class-labelled open functions of 17–96 bytes: state-group `IsRunnable` / `Update` / sleep-window
tests (`IsInSleepTimeWithWakeDelay`, `IsStillValid`), the `JackOfBladesBase` constructor
(`#pragma optimize("s", on)` hoists the shared `CIntelligentPointer<CThing>` vtable), the
`CMeshBankManager`, `CActionDropGenericBox`, `CGameComponent(CGame*)` and `CNetworkClient`
constructors, eight def / render constructors mislabelled "HoverHornetQueen" in the manifest
(`CTavernGameCardBaseDef`, `C3DMeshStripifier`, `CVillagePeopleDef`, `CRenderTarget`,
`CVertexShaderInput`, three no-RTTI script vtables named by address), a `CDataStream`-derived
destructor and an STLport-style `vector<T>::resize(n)` over a 16-byte element. Callee labels
corrected: 0x008FCFC0 `CAIStateGroupBase::GetPWorld`, 0x008FCF40 `::Terminate`, 0x0049D870
`GFGetCurrentFrame`, slot 11 `IsRunnable`.

Lessons recorded by the pass: constructor stores before the derived vtable store are plain
initialisers and stores after it belong to an embedded member with an inline constructor; two
argument stores before the vtable are an inlined `__declspec(novtable)` intermediate base; the
`std::list` constructor stores the head last; an inlined `GetInterface<T>` with an uninitialised
out-pointer local reproduces the miss-path frame read; separate `{ Terminate(); return false; }`
blocks tail-merge with a hoisted `mov ecx,esi` where `&&` does not; `IsStopping() ? false : true`
keeps the `neg ; sbb ; inc` byte sequence; an inline accessor gives `lea` where a direct member
call gives `add ecx,imm`; `enemy.Z > marker.Z + 1.5f` evaluates the left call first with the
literal from the float pool. Three constructors differ by scheduling only.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
