# Chunk 312 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `GFFloatRandom` 0x4978A0 is `float __fastcall (float range, unsigned long* seed)`. The seed travels in ecx, so the GetPWorldSeed call comes before the range push (009175e0, 0097a630).
- Passing a `CThing*` to a `const CIntelligentPointer<const CThing>&` parameter (implicit conversion) builds a temporary that is re-addressed with lea. An explicit `CIntelligentPointer<...>(p)` temporary pushes the ctor's eax instead (0097a630).
- Separate `if (!X()) { Terminate(); return; }` exits give retail's preloaded `mov ecx,esi; je` merged form. An `||` chain of the same tests does not (0097a630).
- Count an STLport vector of 8-byte entries as `(unsigned long)(finish - start) == 0`; this gives retail's `test ecx,0xfffffff8` (0097a630).
- `nd.py` masks large immediates, including `sub esp,N`. Check the frame size with `udiff.py | grep "sub esp"`. A frame-size miss usually means an action object has the wrong size (CCreatureAction_ProjectileCrouch is 0x124).
- Filters that copy the thing's 64-bit id (0x6AF290 / 0x6AF2B0) need an `__int64` member. That member is what gives the 8-aligned ebp frame (0092a3f0).
- A bool that retail keeps in a dead argument slot (`lea ecx,[esp+argslot]` as the `bool&` of several AIAction calls): give each call its own scoped `bool reached`. VC7.1 then packs them into the dead parameter slot; one shared local gets a frame byte instead (00980770).
- Byte-identical exit tails: a mid-function `return result;` cross-jumps to whichever identical tail VC7.1 emits first. Writing the final branch inverted (failure arm first) moved the jump to retail's tail (00980770).
- Retry loops `if (r == 4) continue; break;` stay unrotated with the guard at the loop top, which also keeps the `lea esp,[esp]` / `mov edi,edi` alignment pads (00980770, 00948880).
- `IS_THING_ALIVE` exists in two forms. The inline pointer form is `GetPItem() != 0 && !(flags91 & 1)`. The out-of-line `__fastcall` copies are 0x4CC340 (IP<CThing>), 0x919040 and 0x6961F0 (IP<const CThing>). Match the form each call site uses.
- When a `TestTC` flag is stored to a frame byte before `je`, write `bool has = thing->TestTC(t); if (!has) return; p = GetTC(t);`. That keeps the component pointer in a register with a frame copy in the key slot (00980770). The out-parameter GetInterface form put it in the frame only.
- `const char*` arguments to `const CCharString&` parameters build every temporary before the pushes (0092a8a0, 0097a630), as noted in GOTCHAS.

Review at landing:
- These rows needed more than 4 grades: 0094d020, 0092d1f0, 009482a0, 00980bd0, 0097e380, 009289b0, 0091ec30, 0097a630, 00980770 (6 probes).
- 0092a8a0 uses a local model next to the existing CAIStateGroup_ApprenticeHero engine header.
- 0097a630 uses a local CAIStateGroup_CreatureTakeCover model, because the header flattens the cover base and retail's member offsets need it.
- 00980770 declares IS_THING_ALIVE 0x4CC340 declared-only. It also routes 0x4C73D0 through the CThing* item and 0x6A4D00 through CThingCreatureBase.
- The status rows carry several propagated labels that are wrong; see NOTES.md, Label corrections (e.g. 0x007FEF40 EditCyclePreviousKeyCamera).
