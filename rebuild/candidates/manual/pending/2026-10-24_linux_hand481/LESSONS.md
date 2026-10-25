# Chunk 481 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Read-whole-file buffer** (0x0088B0C0): STLport `std::vector<char> buffer(n)` compiles to:
  - `inc edi; je` → `n ? malloc(n) : 0`;
  - the inline memset fill (`mov edx,ecx; shr ecx,2; xor eax,eax; rep stosd; and ecx,3; rep stosb`);
  - an `if (p) free(p)` at every exit.

  The vector's three words get no frame slot. Recognise it before inventing a raw `malloc`/`free` pair.
- **Implicit vs explicit string temporaries** (0x0088B0C0):
  - An IMPLICIT `const char*` → `const CCharString&` conversion re-leas the temporary for the push (`lea ecx,[t] ; push ecx`).
  - An explicit `CCharString("x")` temporary pushes the ctor's returned `eax` instead.
  - `if (!parser.SkipPastNextItemIfIs(": "))` computes `mov bl,al ; neg bl ; sbb bl,bl ; inc bl` before the temporary's dtor.
  - A named local inside an inline helper also gives the lea, but negates after the dtor.
- **Explicit returns per branch** (0x008F2280): an explicit `return` in each branch of a state switch gives retail's duplicated release and epilogue per branch. An if/else chain cross-jumps the shared `SetState` calls.
- **Same-TU stand-in drops the null test** (0x008F2280, landed 0x008F0B80): a same-TU `__declspec(noinline)` GetCardDef stand-in plus a null-checking CDefPointer dtor removes the null test for a dereferenced def, giving `mov/dec/store` release. A def that is never dereferenced keeps `cmp ecx,edi ; je ; dec [ecx+4]`.
- **Strength-reduced counter** (0x008F2280): `long i = 0; for (...; ++card, ++i) ... (53 - i)` becomes a strength-reduced, decrementing memory counter stored after the hoisted copies. A literal `height = 53; ... --height` is stored before the loop guard instead.
- **`== true` on a bool member** (0x008F2280): `x.Moving == true` emits `mov dl,1 ; cmp byte [..],dl`. A plain `if (x.Moving)` emits `test`.
- **fxch pair before fpatan** (0x00878DB0): `fxch st(1) ; fxch st(1)` before `fpatan` needs atan2's x argument to be a float local (`float xy = (float)sqrt(x*x + y*y); atan2(z, xy)`).
- **Negated side vector** (0x00878DB0): a side vector `(look.Y, -look.X, 0.0f)` negated memberwise into `axis(-side.X, -side.Y, side.Z)` gives retail's double `fchs` and the `+0.0f` Z store.
- **Double 2π pool** (0x008F06F0): a double pool holding `(double)6.2831855f` (0x0124F2B8) is `turn * (double)TWO_PI` with `static const float TWO_PI = 6.2831855f`, not the double literal 6.283185307179586.
- **Division by a constant** (0x00876E10): VC7.1 turns a division by a float constant (`/ 11.0f`, `/ step` with a constant step) into a multiply by the reciprocal pool constant.
- **Local stored before the member** (0x00873690, parked): `float phi = Phi + d; Phi = phi;` stores the local before the member. A temporary `C3DVector(Base.X, Base.Y, Base.Z + h) + dir * r` evaluates its constructor arguments first.
- **STLport `_Rb_tree` ctor and back() re-fetch** (0x00881C90, parked): the STLport `_Rb_tree` base ctor allocates the header before the `_M_node_count` store. Re-fetching `stack[priority].back()` at each use matches retail's reloads; a cached reference does not.
- **`/Oa` TU** (0x00861E60): a string-data pointer loaded once and kept in esi across two CRC calls is an `/Oa` TU. Same-TU bodies for `_M_insert_overflow` and GetSubDef keep the call shapes.
- **Empty user dtor keeps the vtable store** (0x008814C0): a user-declared empty virtual dtor keeps the vtable store; a generated one drops it.
- **Clamp on the argument slot** (0x00882FE0 / 0x008854A0): `if (t < 0) t = 0; else if (t > 1) t = 1;` clamps on t's own argument slot. A user default ctor on the by-value result puts the hidden pointer in ecx.
- **Unset out-pointer** (0x00864D00 / 0x008F4990): `if (HasTC(n)) p = GetTC(n);` into an otherwise unset pointer reproduces retail's absent-path read. With everything under one `if`, a single shared `return false` follows.
- **Leaf must be a symbol prefix for calls.py** (0x008F06F0): `// Leaf:` must be a PREFIX of the symbol, e.g. `?FrameUpdate@CCard@@`, for calls.py. A bare `FrameUpdate` grades under q.py (substring) but calls.py finds 0 blocks. Also declare engine types as `class` (`ABVC3DVector@@`) so the mangled name matches the PDB.
- **fmod overloads in VC tests**: under VC7.1's `<cmath>`, `std::fmod(float, double)` is ambiguous (C2666 → TCC_FAIL in tall.py) while g++ accepts it. Cast both arguments to double in tests.

Review at landing:
- No row needed more than 4 grades. The maximum was 4: 00864d00, 00876e10, 008818a0, 00888a60.
- Local base models sit next to existing engine headers in these src rows; retype them at landing:
  - 0085ff10: CAIBrain, CCreatureActionBase, CThingCreatureBase
  - 00861e60: CAnimFilterNoGroup, CDefClassBase, CDefinitionManager, CThingCreatureBase
  - 00864250 / 00864700 / 00864d00: CTCPerceiveThing, CThingAICreature, CThingCreatureBase
  - 008643a0: CDefClassBase, CDefinitionManager, CTCEntityEvents, CThingAICreature, CThingCreatureBase
  - 008648a0 / 00864b30: CDefClassBase, CDefinitionManager, CTCCreatureOpinionOfHero, CTCVillage (008648a0 only), CThingAICreature, CThingCreatureBase
  - 00870530: CTCBase
  - 008b7cf0: CCreatureAction_UnsheatheWeapons, CDefClassBase, CThingCreatureBase
  - 008df510: CCreatureAction_ScorpionTailWithdraw, CDefClassBase, CThingCreatureBase
  - 008f2280: CDefClassBase, CTCTavernGame, CTavernGameCardBaseDef
  - 008f4990: CThingCreatureBase

  The models only touch a few members plus out-of-line calls. CStringParser (differ 0088b0c0) is local on purpose: retail is 0x28 bytes, the header is 0x30.
- The relocation-masked grader does not check jump-table order. 008b7cf0 depends on case 5 being written before case 4.
- Errors in already-landed sources:
  - The landed 0x00830F20 "HasAnimation_slot73" is AnimExists.
  - Interface 0xAD is named both CTCRushCounter and CTCAD.
  - The BattleChargeTurnLeft "SetAnimationName" is CCombatActionBase::SetCombatAnimation (0x008574D0).
  - The landed 0x00869020 calls 0x00868E50 "GetArmourDefsForThing"; the PDB name is GetArmourWeightsFromThing.
  - The landed CTCTavernGameCoinBase::UpdateMeshes declares PositionMesh's 5th parameter as `long`; CCard::FrameUpdate 0x008F06F0 shows it is a float (tilt).
  - The landed CFirstPerson::OnInit calls 0x00A14440 "Normalise"; it is C3DVector::AccurateNormalise.
  - Landed sources name 0x009B9BA0 both NextItemIs and FindKeyword.
- No catalog row in this chunk holds several functions.
