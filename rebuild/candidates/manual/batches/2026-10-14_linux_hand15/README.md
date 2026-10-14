# Batch `2026-10-14_linux_hand15` — hand-authored CBasicShape .. CCachedFile functions: 40 bodies (39 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the sixteenth chunk of
class-labelled open functions of 17–96 bytes: def-module constructors (`CCoopSpiritDef`,
`CBedDef`, `CSpecialAbilitiesForcePushDef`, `CShaderDataBank`), the `CCardDef` copy / assign /
virtual `Copy` trio, `CDefPointeeBase`-derived destructors, the `CBasicString<char>`
constructors, a `CBinaryFileDataWriter::Write`, a `CRegisteredTCTable` registration
(`pair<CCharString, CRegisteredTCInfo>` push_back), a render-device vertex-buffer draw
(`IDirect3DDevice9` `__stdcall` slots), `CBulletTime` setters and the SSE2 `CBitfield` shifts
(`__declspec(align(16))`, `/GS` cookie frame). Many manifest labels were propagated and were
corrected from the RTTI vtables (listed in the pass notes and each header comment).

Lessons recorded by the pass: a constructor whose vtable store sinks below all member stores
while a `CDefString(-1)` member keeps source order is a two-level `__declspec(novtable)` chain;
`memset(this, 0, sizeof *this)` in a local's constructor gives the `xor eax,eax` zero block; three
member copies that re-read a global between stores are three scalar statements; `x*b + y*b` is
factored unless the products are named float locals; a non-polymorphic `??_G` needs an implicit
destructor plus a `delete p` helper; the SSE2 shift bodies reproduce with `_mm_sra_epi32` and
`/GS`. One body differs (`CBulletTime::SetBulletTimeSlowdownMultiplier`: the priority argument
is stored to a frame temporary before the map find-or-insert call).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
