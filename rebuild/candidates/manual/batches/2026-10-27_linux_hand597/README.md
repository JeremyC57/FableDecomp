# Chunk 597 (2026-10-30): 8 of 10 engine-range released pass 9 rows landed (7 C++ + 1 retail_asm)

7 C++ rows (1 MATCH, 6 RELOCATION_MATCH) and 1 grade `retail_asm` row, all PASS (g++ and VC7.1). Proven from both PDB
name sets plus a second source: `ImeUi_ToggleLanguageBar` 0x00CB3570 (DXUT; `bool` not BOOL), `LoadDataFromMarker`
0x00CD5170, `CParticleProgram::Clear` 0x00AE3360,
`CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles` 0x00B9E750 (uses
engine/CIndexBuffer.h and CTexture.h; its matching form came from compile-only probes after the 4-grade budget, confirmed
by gall.sh and vland); from public source plus callers: zlib 1.2.1 `longest_match` 0x00C00E60 (MATCH; the TU carries a
reduced `deflate_fast` caller only so the static gets its register convention: never land it as deflate_fast) and STLport
`std::__rotate<CEngineSubPrimitiveRenderer::CSortElement*, int, CSortElement>` 0x00B85660. Descriptive (libpk, no PDB):
`CAmbientTrigger::Play` 0x00C62530 (stand-ins for 0x00C62350 SetLungedThisFrame and 0x00C62370 GetRandomPointInSphere in
the TU; the latter byte-matches retail and can be its own row) and `CAudioContext::GetPreferredDeviceName` 0x00C55DA0
(retail_asm_payload.json / retail_asm_oracle.tsv: retail brackets the two winmm calls with `pushad` 0x00C55DB4 / `popad`
0x00C55DDA, bytes 60 / 61 verified in Fable.exe; VC7.1 never emits pusha/popa from C++; purity.classify = retail_asm;
/GS). Five of the ten manifest labels were wrong.
Parked (../../pending/2026-10-27_linux_hand597/differ/): 0x00C298D0 `vector<CHandleBasedMemoryPool::CHandleInfo>::_Insert_n`
352v542, no instruction differs: the catalog row stops at the first `ret 0Ch` but the body runs past the Catch@00c29a30 /
Catch@00c29a3c funclets to 0x00C29AEE; graded over 542 bytes with /EHsc /Gr it is RELOCATION_MATCH (BOUNDS_FIX.tsv now
says `len 542`: re-cut and merge the two Catch rows). 0x00CBF13C `GetBestThingFromMarkerList` (both PDBs) 355v355, one
register choice (retail target in edi / origin in ebx, ours reversed; 16 instructions differ by register name only; 3
grades + ~40 probes in NOTES.md; next: change how often target is used).
NAME_FIXES.tsv: 25 rows (CCharString ctor/copy/dtor/operator= 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0 / 0x0099EFB0 and the
CScriptThing set 0x006E7B40 / 0x004ABE90 / 0x004AA840 / 0x008AB980 as PDB spellings; 0x00CDBDC0 `_M_insert_overflow` is IAE;
0x00996B60 `CMemoryDataOutputStream::Clear` (no Reset in either PDB); 0x00A62ED0 `CMovableResourceMemoryPool::FreeResource`;
the libpk sound family 0x00C62440 / 0x00C62500 / 0x00C13140 / 0x00C383D0 and the float setters 0x00C20420 / 0x00C20480 /
0x00C204A0 all landed under one name `CBrainOutput::SetAsPumpCombatActions`; C3DVector operator+ / GetMagnitude...) + 3
coordinator review rows (0x00C62370 as its own row; CHandleTable vs CSoundSystem_00c37840 and the three play-parameter
class names; the epsilon 0x0129BA3C with ~49 spellings).
Header gaps: none for C3DPrimitive2, C3DTextureStages, CStaticBlock, CRenderManagerCore, CShaderRenderManager, CCharString,
CDataMarkerOutput, libpk / DXUT / zlib types; CParticleProgram.h flattened; CMovableResourceMemoryPool / CMemoryDataOutputStream
/ CMovableResource headers method-less; CRenderStateManager.h only the Ego_r base; CEngineSubPrimitiveRenderer.h lacks the
nested CSortElement. `CDataMarkerOutput::CElement` defaults DataAsULONG to 10.
Lessons (LESSONS.md): a same-TU `static __declspec(noinline) __stdcall` helper returning a class by value gets retail's
private convention (result pointer in esi, float on the stack, `ret 4`); an x87 value kept on the FPU stack across a call
means the callee is defined earlier in the TU; `if (x <= 0.0f) return;` gives `test ah,41h ; jnp`; a member `Dot` returning
`const float` gives retail's Y, X, Z term order; CCharString inline accessors with the null case first give retail's branch
order; STLport's `__false_type` tag needs a user ctor or VC adds a zero store; hoisting the loop count into a local gives the
count-down loop and the order of named argument locals decides which value lands in ebx; an always-throwing `_Xlen` plus
`operator delete(void*) throw()` removes the leftover bytes and the state -1 store in Dinkumware `_Insert_n`; grade over the
true extent before parking an over-span row.
