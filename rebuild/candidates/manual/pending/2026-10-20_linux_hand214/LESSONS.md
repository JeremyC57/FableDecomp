# Chunk 214 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The GetTC end/hit choice written as one ternary (`return (T*)((it == last || k < it->type) ? last : it)->component;`) lands the component straight in ecx for the next call; the if/assign form keeps it in eax and adds `mov ecx,eax` (0x00663E90, 0x00809030).
- A def read out of a GetSubDef out pointer and released on every exit matches as a small holder (`CHeldDef<T> def(out.Object)` with an inline null-tested release dtor) declared AFTER the other call that precedes it; the release then stays `dec [esi+4] ; jne` (0x00662CC0). Holders or locals left in volatile registers keep the long `add eax,4 ; dec ; test ; mov [eax],..` form.
- A by-value CDefPointer copy (`CDefPointer<T> def(player->PlayerDef)` with an add-ref copy ctor and an inline Release dtor) reproduces the inc-with-null-test / `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne` pair (0x006A9D10).
- `if (!restricted) return f(name, Temp()); return false;` puts the restricted false exit last; nested `if (has) {...} return false;` likewise moves early-false blocks behind the body (0x00662CC0, 0x006A8930, 0x00960FC0, 0x006A8240).
- A frame-pointer-free float compare chain: `(a - b) > eps` folded into one expression gives retail's `fsubr [spill]`; a named second operand gives `fld ; fsub st(1)` (0x008098B0).
- VC7.1 ranks `X*X + Y*Y + Z*Z` per operand location: Z,X,Y for a frame-spilled delta and Z,Y,X for one on the x87 stack -- write the plain expression, not a statement chain, when both deltas share one helper (0x007A2F90); a single stack delta that needs Z,Y,X written as a chain wants `s = Z*Z; s += Y*Y; s += X*X;` (0x008090E0).
- `lenSq <= r*r ? tail-call : false` gives `test ah,1 ; jne false` (0x00665190); `!(Health <= 0.0f)` gives `test ah,41h ; jnp kill` (0x00704150).
- `(unsigned long)((uid >> 40) & 0xffffff)` keeps retail's `shr 8 ; and 0FFFFFFh`; going through a 32-bit high-dword local drops the mask (0x0051E210).
- A string literal for a `const CCharString&` parameter re-addressed with `lea ecx,[esp+N] ; push ecx` after its ctor is an implicit conversion (ctor with defaulted length); the explicit temporary pushes eax (0x00799D80, 0x007038D0).
- A bool member or local read once into bl is a named `bool x = Member;` (0x00704150); a vector element read before the empty test is `*v.begin() == k && !v.empty()` (0x00663E90).
- Engine headers with no methods are used through `#define C C_PDB / #include / #undef C / struct C : C_PDB { methods }` (0x00637E70).
- C2DCoordI corner locals: declaration order decides the copy-store order even when frame slots are fixed (0x00637E70: bottom-left before top-right).
- A missing `CThing` tail pad shifts every later member: a bit-field byte at +0x91 in a flat CThing model needs explicit padding to 0x94 before a derived class's members (0x006622D0).

Review at landing:
- 0x007F6BF0 / 0x007F6CB0 are byte twins assigned to <CThingAICreature> / <CThingCreatureBase>; the assignment of T to address is unproven.
- Local base models are used next to existing engine headers for CThingCreatureBase (header carries PDB offsets; retail is 0x1c..0x28 lower) and CThingAICreature; documented in each file.
- Landed-tree name conflicts touched here: 0x00A01B50 GetPItem return type (CThing* vs CBaseObject*), 0x0044C6B0/0x004196B2 (CGameDefinitionManager/CDefinitionManager_Calls vs CDefinitionManager), 0x0045DD2C (three class names), 0x00666B50 (CThing vs CThingCreatureBase), 0x0087C420 (IsOfType/HasProperty), 0x00770DE0 (CTCSolidToThing::IsSolidToThing vs CTCCollisionExclusion::CanCollideWith), 0x0042971D (four rebalance spellings), 0x01375550 g_framesPerSecond (used here), 0x013B8394 (g_pSoundSystem as CSoundSystem*).
- Several parked rows (00662ef0, 006648d0, 00664ba0, 007f6df0) are worth one retry with the CHeldDef holder shape that matched 0x00662CC0.
