# Chunk 377 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- int3 after a final noreturn call: when a body ends in a noreturn call (STLport `_M_throw_length_error`), VC7.1 emits a 0xCC that belongs to the function. The chunk row and q.py strip it as padding, so the grade reads DIFFER(N+1vN), but an N+1 oracle row read from Fable.exe gives RELOCATION_MATCH (0x568E70, 0x568F10).
- STLport `operator+(string, string)`: `_Str r(_Reserve_t(), n)` needs the `_Reserve_t` ctor `__forceinline`. Declaring `abort` noreturn makes the length-error cold block end in `jmp abort` (0x569690).
- Same-TU `__declspec(noinline)` stand-in bodies for read-only callees let VC7.1 prove that locals do not escape. It then propagates constants, keeps values in registers across the call, and homes `__false_type()` / tag temporaries in dead argument slots. Declared-only callees give `[ebp-1]` temporaries and a bigger frame (0x567240, 0x569E30, 0x579A4B, 0x579B3C, 0x58044A).
- `map::operator[]` (`insert(i, value_type(k, T()))`): the vector copy ctor and `insert_unique(hint)` as same-TU noinline bodies elide the `T()` dtor and cache the zero register. `#pragma auto_inline(off)` is the wrong lever: it also un-inlines the pair ctor (200v188) (0x56A680).
- `std::operator!=(string, const char*)` is STLport's `!(s == p)`: materialised 1/0, then `test al,al ; sete cl` in each tail. The landed caller 0x5615C0 calls it operator==, but the body is the `!=` form (0x566EA0).
- STLport `__copy` / `__copy_backward` / `fill` / `__uninitialized_fill_n` rows land verbatim as `__fastcall` free templates (with `random_access_iterator_tag`, `_Distance*`). Grade them with the `??$` leaf (`??$__copy`, `??$__copy_backward`, `??$fill`, `??$__uninitialized_fill_n`) (0x578C50, 0x578CF7, 0x57F279, 0x578C8D).
- Member templates mangle with `??$`: `vector::_M_allocate_and_copy<It>` is `??$_M_allocate_and_copy@...`, so the leaf `?_M_allocate_and_copy@` matches nothing in `objdump -t` (0x5796D1).
- `_Rb_tree::erase(iterator)` over a value with an implicit non-trivial dtor (`pair<const K, SExpSpendUndo>`): VC7.1 destroys it through the pair's `??_G` with 0. Instantiate only `_Rb_tree`, not a map wrapper (which emits a second `?erase@`), and use the leaf `?erase@?$_Rb_tree@` (0x579D18, 0x579D55).
- Size TU float constants: under `optimize("s", on)`, `= 1.0f` stores become `fld1 ; fstp` and other immediates load from `.rdata`. A speed TU gives integer immediate stores (0x56ED74).
- `float ceil(float x) { return (float)ceil((double)x); }` is a speed TU (`sub esp,8 ; fstp qword [esp]`). Under "s" it becomes `push ecx ; push ecx` (0x57A2D0).
- `fdiv st,st(1)` comes from an in-place divide: `float f = (float)a; f /= max; return f;`. `a / max` gives `fidivr` and a named quotient gives `fdivrp` (0x57A57A).
- A class returned by value with a user copy ctor and dtor goes through the hidden pointer; without them VC7.1 returns a 4-byte class in eax (CWideString, 0x57AD4C).
- Load order and registers follow the source: a named local `CDataBank* bank = g_pWorld->PDataBank;` gives the early load, and a second read through an inline accessor `g_pWorld->GetPDataBank()->...` gives the later `mov ecx` (0x57AD4C).
- Under /Os, trivial inline members of traits structs stay out of line unless `__forceinline` (0x57F78F, also the CIVCountedPointer dtor in 0x57AD4C).
- `out += CCharString(*u64ptr)` pushes both words from memory; retail's `mov eax,[esi] ; push eax` for the low word comes from copying to a local first: `unsigned __int64 copy = *value; out += CCharString(copy);` (0x57F78F).
- A two-way literal pick (`if (b) out += "TRUE"; else out += "FALSE";`) tail-merges into one call, and relocation masking cannot tell which literal goes with which arm. Check the pairing with `objdump -dr`: TRUE on the fall-through, FALSE on the `je` target (0x57F78F).
- `GFSerialiseVectorBinaryIn/Out<T>` traits: `{ T DefaultVal; TransferBinaryIn/Out(stream, T*) }` with the read-only body as a same-TU noinline stand-in; VC7.1 then aliases the empty traits object onto the count's frame word (0x569B30, 0x57F109).
- Behaviour tests: VC7.1 at /Od does no NRVO, so a test model that returns a named counted-pointer local by value needs a copy ctor that adds a reference. Otherwise the local's dtor releases twice, and the test passes under g++ but fails under tall.py (test/0057ad4c.cpp).

Review at landing:
- Rows that needed more than 4 grades:
  - 0056fbed: about 13 grades, parked.
  - 0056a680: 5 grades, matched.
  - 0057a57a and 0057ad4c: 4 grades each, matched.
- Local models next to existing engine headers:
  - 0058044a defines CHeroMarriageInfo locally: it needs the user copy ctor (0x57A3A2), which engine/CHeroMarriageInfo.h lacks.
  - 0057a57a uses a retail CTCHeroStats view (Stamina +0x58, MaxStamina +0x5C, 8 bytes below the PDB header).
  - 0056ed74, 0056fbed, 00578c50, 00578c8d, 00578cf7, 00579a4b and 00579b3c declare a local CTCInventoryExperience scope, for the method or for the nested ExperienceSpendInfo mangling; the generated header has no nested ExperienceSpendInfo.
  - 00569c50 uses a local LHHttpHeaderStatus with the retail 0x48-byte STLport layout; the header is the 0x8C Ego_r (Dinkumware) layout.
  - differ/0056fbed uses a local CThing model.
- Errors in already-landed sources:
  - Landed 0x596CCE declares JustGetTheFrikkingText2 at 0x0099B6A0. That address is CWideString(const CCharString&); JustGetTheFrikkingText2 is 0x57AD4C.
  - Landed 0x5615C0 calls 0x566EA0 operator==; it is operator!=.
  - Landed model for 0x449960 is named "Catch@0040d959"; it is CPlayerManager::GetMainPlayer() const.
  - Landed 0x57A3A2 (CHeroMarriageInfo copy) uses a struct-MI hack.
  - Landed 0x5799DE is labelled CLandscapeBackgroundPatch; it is pair<const K, SExpSpendUndo>::`scalar deleting destructor'.
- Catalog rows holding several functions: none in this chunk.
- 00569a80 keeps the landed `CRetailTree_erase00569a80::Erase(node*)` symbol (as its sibling 0x56DE50) rather than an STLport `_Rb_tree::_M_erase` spelling. Rename both together if the catalog convention changes.
