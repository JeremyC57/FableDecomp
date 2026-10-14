# Batch `2026-10-14_linux_hand28` — delegated hand-authoring chunk 28 (CDateAndTime .. CDisplayManager): 38 of 40 landed

Chunk 28 of the class-labelled 17–96-byte pool, authored by the session's single sub-agent (`$S/w22/BRIEF.md`) and
landed by the coordinator: 38 rows MATCH/RELOCATION_MATCH with behaviour PASS under VC7.1 + Wine (00c7b810 / 00c7b840
under `/GS`; three /Os-region bodies carry `#pragma optimize("s", on)`). SSE vector constructors (`__m128i` by value in
xmm0, `_mm_set_ps`), SGI `vector::erase(pos)` instantiations, `_Uninit_fill_n` over packed def classes, retail-tree
`Contains`, CDefStringTable / CDefinitionManager / CDiskFileWin32 / CDisplayManager helpers.

Label corrections (details in `$S/w22/agent28/NOTES.md`): 00c7b810 / 00c7b840 "CDateAndTime" are `CVectorSSE(__m128i)`
constructors, 00c88930 a four-float SSE constructor; 007467d0 / 009e4aa0 "CDefString::ToWideString" are `Register()`
forwarders on list-like owners; 005e1e60 / 00720810 "UnloadResource" are `vector::erase(pos)`; 0067ca70
"CopyConstructRange" is `_Uninit_fill_n<CBlurredAttitudesDef*>` (landed with that leaf, module `_global`); 006e5660 /
007394c0 / 0079dd80 "CopyBackBufferToTexture" are `long Contains(long)` on retail trees; 00a04410 is a 0x2c-byte
vptr-less record (memset of its block).

Lessons (also in GOTCHAS, wave 24d): a by-value `__m128i` arrives in xmm0 and makes `/GS` build an aligned cookie frame;
the SGI erase's const-ref empty tag is homed in the dead `pos` slot; a CCharString returned by value and chained, or a
pointer built from a call result passed with a literal, must be a named local; a tree `Contains` takes its key by value
(the slot's address feeds `find`); `inc dword ptr [ecx+4]` AddRef = inline helper, the load/inc/store form = direct
`RefCount = RefCount + 1`; one member zeroed first then the rest = that member alone in the init list; a pointer-to-member
call through {function, this-adjust} needs `__multiple_inheritance` on the forward declaration; an inline accessor keeps
COM interface loads after the table lookups; `Opened = false` in the body vs the init list moves the byte store across
the CWideString constructor call.

Not landed (best sources in `$S/w22/agent28/differ/`): 00a31220 (`__fastcall swap` of a 20-byte record: register allocation),
009ace90 (CDefClassBase::UnloadResource: `return false` tail placement, the 0x00CA67B0 gap).
