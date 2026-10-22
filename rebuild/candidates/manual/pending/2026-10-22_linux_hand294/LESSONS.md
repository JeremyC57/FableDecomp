# Chunk 294 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Entity-script cutscenes: use the landed CBowerstoneIntroGuard::Main 0x00D246C0 model (destructor-less handle views, explicit Destroy() 0x7E74D0 / DestroyInline() at every exit, `CGameScriptInterface* game = Game;` for the paired PauseAllNonScriptedEntities). A handle retail constructs out of line (call 0x7E72A0) must be a storage view with `Construct()`; a by-value handle with a ctor emitted the CBaseObject ctor twice (CPrisonRaceWarden 0x00DDD030).
- `for (;;) { if (term) { inline destroy; return; } if (flag) break; NewScriptFrame(); }` gives retail's single merged inline exit; use `do NewScriptFrame(); while (!term);` for a final idle loop only when retail has no initial test (otherwise VC duplicates the top test, see 0x00DDC070).
- VC7.1 hoists a terminate test duplicated in both if-arms above the if; keep it in each arm only when retail repeats it.
- Frame/register choice follows lexical scope: block-scope an out-local and copy it to a register local (`if (!Locate(&h) && !g) return false; site = h;`) to get retail's `test al,al`.
- Named locals fix argument evaluation order (`long def = ...;` before the Create call) and early loads (`long number = Number;` gives `mov edi,[ebp+0x28]`; a plain member compare emits `cmp [mem],eax`).
- x87 product operand order is canonicalised by VC regardless of source term order or parenthesisation (0x00A42470, 0x00A12450, 0x00A120D0): treat as a lane gap after 3 spellings.
- An aligned frame (`and esp,-8`) can appear in our build where retail has none (0x00A6D670); cause unidentified, look for an 8-byte local.
- Carried from earlier sessions: a named C2DBoxF local (a temporary schedules its zero stores early); push_back with a `prev` local; raw bitset word test; inline STLport list erase writes free() (0x00BFEA14); `i != size()` loop bounds; `Z + (X + Y)` sum order; inline_depth does not affect a loop-scope vector's dtors.
- Tooling: gd.sh needs an absolute source path; ud.py BrokenPipe under `head` is harmless; gsi.py treats `//` inside string literals as a comment (keep URLs/paths out of string args when probing).

Review at landing:
- 0x00A6D0E0 / 0x00A6D670 differ files model CPhysicsMesh with a local struct (no shared header exists); the offsets are from the disassembly.
- 0x00459BB6 needs `#pragma optimize("s",on)`; check the verifier accepts it.
- The label corrections in NOTES.md (CTriangleList / FindFirstBone / DoSizeof / CCarriedReadableDef manifest names are byte-fold propagations).
- 0x00CFA700 / 0x00D12D00 / 0x00D1CC40 use the PDB member names *NakedBoastReward and the string TEXT_QST_BOAST_DESCRIPTION_NAKED; a case-insensitive purity scan would flag them (case-sensitive scan is clean).
