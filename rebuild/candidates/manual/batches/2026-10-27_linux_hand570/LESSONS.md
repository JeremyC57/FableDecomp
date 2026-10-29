# Chunk 570 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `__declspec(align(16))` local + `__asm` SSE kernel -> retail's ebx-realigned frame; the `mov ebp,[ebx+4] ; mov [esp+4],ebp` return-address copy appears only with **/Zi** (`// Flags: /Zi`), it is not a lane gap (0x00A5BEF0; contradicts chunk 511).
- NVectorMath x87 fallbacks: copy the stream pointers into locals and make the lerp a `__forceinline` member; VC7.1 then unrolls by four like retail (0x00A5BEF0).
- VC7.1 instantiates class-template member bodies at the end of the TU, so `#pragma inline_depth` around a template definition is ignored (the EOF pragma state applies); to keep one inlined template member's nested calls out of line, define an explicit specialization of that member under `#pragma inline_depth(0)` (CSpawnedFunc1 ctor in 0x00EF87D0: by-value parameter dtor 0x004AA840 out of line while the caller's own CScriptThing dtors stay inline).
- Statement-level `#pragma inline_depth(n)` does not limit nested expansion of callees already inlined; `#pragma inline_depth(1)` before the function limits depth-2 ctor chains (base ctors go out of line) but did not stop a member copy inside an inlined ctor (0x00EF87D0 probes).
- Inline copy of Me into a by-value spawn argument + out-of-line 0x004ABE90 for predicate/functor members = const out-of-line copy ctor plus `__forceinline CScriptThing(CScriptThing&)` (Me is a non-const lvalue; `const P1 par1` takes the const one) (0x00EF87D0, as CCorneredGuard 0x00DA7830).
- Inline `cast["Hero"] = hero` on a std::map of ScriptedThingBase needs `__forceinline` on both the ScriptedThingBase operator= and CCountedPointer::operator= (retail reads other.Info, then other.Data, before the self-compare) (0x00E295E0).
- Quest-script Main (UAE) needs a polymorphic CScriptBase (`virtual ~CScriptBase(); virtual void Init(); virtual void Main() = 0; unsigned char _body[0x3c];`), otherwise `this`-relative offsets shift by 4 (0x00EC3D40).
- CGameScriptInterface models must keep their padding virtuals; filtering a donor model with grep drops them and silently shifts every slot (0x00EC3D40).
- STLport vector bodies: same-TU `__declspec(noinline)` stand-in bodies under `#pragma auto_inline(off)` remove dead temp dtors; an explicit member specialization keeps vector<long>::_M_insert_overflow (0x00440990) out of line (0x00F3E6B0 section 1 exact).

Review at landing:
- 0x00F3E6B0 used more than 4 grades (parked; differ/00f3e6b0.cpp).
- Rows 00e295e0 / 00ef87d0 model CScriptGameResourceObjectScriptedThingBase locally next to the flat engine header (header debt in NOTES.md).
- Row 00ef87d0 carries a modelling-only inline `CScriptThing(CScriptThing&)` (not emitted; retail has one copy ctor 0x004ABE90, the inline site is most likely an LTCG/heuristic expansion) and an explicit specialization of the CSpawnedFunc1 ctor; the primary template ctor keeps the PDB parameter list except top-level `const` on par1 (not part of the emitted name since the ctor is inlined).
- Already-landed: 0x00E292C0 CMeetSisterMessenger::Main is outside NScript::CQS_MeetSisterScript (NAME_FIXES.tsv); 0x00F35B10 / 0x00F35B30 / 0x007E7300 / 0x00E241B0 / 0x0099A380 / 0x00A5B870 / 0x00A5BBA0 are bound under several wrong spellings (NAME_FIXES.tsv).
- Landed CTriangleStripifier2::CalcSectionScore uses a flat public CArray model, not the STLport vector one.
- Catalog: the CV_RockTrollFirstEncounterScript Alloc factories (0x00EC41F0, 0x00EC4490, 0x00EC45C0) carry the propagated "CTriangleList" label.
- 0x00A5BEF0 lands from retail_asm/ (not in payload.json; finish.py reads src/ only); graded separately in tmp/payload_all.json.
