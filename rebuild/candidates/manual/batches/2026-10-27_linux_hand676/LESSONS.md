# Chunk 676 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- zlib trees.c: define the trees.h tables (static_ltree etc.) IN the TU with their initialisers. VC7.1 folds `static_ltree[END_BLOCK].Code/Len` into immediates (`_tr_align` 0x00C055D0, 497v497); an extern declaration loads them from memory (545v497). tools/gentrees.py regenerates the tables and checks them byte-for-byte against retail 0x012AED48..0x012AF708.
- libvorbis lsp.c statics: `extern "C" { static __declspec(noinline) int __stdcall Newton_Raphson(...) }` plus a same-TU caller gives retail's private convention (`ord` in esi) AND its callee-pop `ret 8` (0x00CA3F60). Plain cdecl differs only in `ret`. So "register arg + ret N" in the Xiph range = static __stdcall, not an LTCG gap.
- The address-range rule for libpk's libvorbis extends to lsp.c at 0x00CA3A40..0x00CA4560 (cheby, comp, Laguerre_With_Deflation 0x00CA3B80, Newton_Raphson, vorbis_lpc_to_lsp `__fastcall` 0x00CA4160).
- An implicit `CWideString(const unsigned short*)` conversion for a `const CWideString&` parameter is constructed in the outgoing argument slot BEFORE the trailing `push 1`; an explicit named temporary or `.ToWideString()` puts it after (0x00ADD4A0).
- `new CThreadedFile` pushes sizeof: model the class's real members (0x1C), never a bare vtable stand-in (`push 4`), even when only virtual calls are used (0x00ADD380).
- A destructor's `delete it->second` loop over a map keeps the header in a callee-saved register only with `iterator end = m.end();` before the loop (0x006C5D70).
- A data pointer local (`COptimisedFace* faceData = &Faces[0];`) declared before an STLport list local reproduces retail's Faces load ahead of the list sentinel construction (0x00AC8170).
- An STLport `map::clear()` used directly by game code must exist as `void clear() { _M_t.clear(); }` in the local map model (0x006AD260).
- Quest-script C3DVector members are byte-aligned (Ego_r +0x8A / +0x99), so `#pragma pack(push,1)` on C3DVector only; without it every later member is 4 high (0x00DBEFC0, also GOTCHAS 0xDB5260).
- Destructors with real STLport text plus engine headers often match on the first or second grade (0x006661B0, 0x006C5D70, 0x00DBEFC0). Settle the class through the slot-0 ??_G, the vtable, the ctor, the Alloc and the registration string first.

Review at landing:
- 0x00D25410 is landed as ??0CQ_NewOakValeIntroScript but is ??0CQ_DragonBossFightScript (Alloc 0x00D27240 push 98h, S_QDBF). The landed file rebuild/src/compiled/00/d2/CQNewOakValeIntroScript_CQNewOakValeIntroScript_00d25410.cpp and the binding ??_7CQ_NewOakValeIntroScript@@6B@ -> 0x012CB70C need respelling (NAME_FIXES.tsv).
- 0x00669B50 bound as ?base_read@@YIXPAX@Z; 0x006AD700 ControlledMove QAE -> IAE; 0x006C4F80 CUnproven placeholder _M_erase; 0x00ADD380/0x00ADD4A0 bound with SFileSpec placeholders; 0x00AC8170 bound without namespace/access/list type.
- Rows over 4 grades: 0x00BA48F0 (22, parked), 0x00ADD380 / 0x00ADD4A0 / 0x00AC8170 (4 each).
- Local models next to existing engine headers: CLevelScriptBase (header declares a dtor), C3DVector pack(1) for quest scripts.
- The unlisted 10-byte function 0x00ADD5C0 between the CLHFile ctors and the ??_G pair has no catalog row.
- The CLHFile ctor names have no PDB symbol. The class comes from RTTI and the parameter types from the callers and the bytes, so the spellings are evidence-best rather than PDB-exact.
