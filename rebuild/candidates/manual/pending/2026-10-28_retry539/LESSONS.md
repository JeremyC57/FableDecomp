# Chunk 539 lessons (GOTCHAS one-liners)

- A same-TU `__declspec(noinline)` body of the real templated callee (GetPFirstLeafThing 0x5800D6) changes VC7.1 frame packing. In 0x57DF85 the out-param marker word started sharing the AddFloat argument homes, and ndiff fell from 32 to 17.
- STLport `vector::operator[]` written as `*(begin()+n)` makes VC7.1 load `_M_start` before an intervening call, where raw `_M_start[n]` loads it after. In 0x5542F0 the begin() form took ndiff from 48 to 31; in 0x555A90 the raw form was better (22 -> 16). Try both.
- Two sibling CGameEvent objects only share frame space when each sits in its own `{}` block. Without the braces the frame grows by 48 (0x57DF85).
- Replace invented stand-in callee bodies with the real landed bodies, ported under their real (templated) names. Use explicit `template <>` specialisations; a partial fake gets C2768. The grade stayed the same (0x584B86 at 103) and the source became legitimate.
- CComponent vtable 0x0124608C slots 66-79 are the AddAction family in PDB definition order (74 AddActionOnDropped, 75 AddActionOnDroppedNowhere); slot 80 OnLeftUnclicked is out of order.
- The CTCBase getters in retail follow PDB order: GetWorld 0x686A70 (IBE AAV), PeekWorld 0x686A80 (IBE ABV), PeekWorldMap 0x686D20, GetWorldMap 0x686D30, GetThingSearchTools 0x686D80, PeekThingSearchTools 0x686D90. Declare them on a CTCBase base class, or they mangle under the derived class.
- A stray `#pragma optimize("s", on)` turns a magic-multiply divide into idiv and adds a frame pointer; one 727-byte row shrank to 228 instructions. Never carry the size pragma over from a sibling TU without retail evidence.
- CDeletion's `Method = 1` store is scheduled before the vtable load when the component comes from a map node (`it->second`). When the component is held in a register (0x584B86) it lands after, as retail has it. The 4-diff residue in 0x5726A0 has no source lever.
- `push ecx` / x87 `fsubp` order in ChangePosition(C2DVector) argument builds is a shared lane gap (0x576C4F, 0x576ED1, and the parked 0x47E651 / 0x47F30F / 0x47E9AE).

## Review at landing
- The same-TU bodies 0x5800D6 (in 0x57DF85) and 0x42B040 / 0x5936C2 (in 0x584B86) may raise RELOC_DUPLICATE against the landed leaves once those rows match.
- The landed 0x5800D6 leaf is spelled under CThingSearchToolsMethods; NAME_FIXES gives the PDB template spelling.
- The 0x42ABCA `list` vs `_List_base` dtor spelling, and the 0x429160 / 0x4291DE dtor vs ReleaseData spelling, are still open.
