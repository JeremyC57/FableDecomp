# Chunk 641 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A CTimer local, not explicit Register/Deregister calls** (0x00D910B0). The destructor at every exit makes per-exit
  DeregisterTimer copies; VC7.1 then hoists the shared `mov ecx,[g_pGameScriptInterface]` out of an early return's copy and
  cross-jumps it into the loop exit's copy (`mov ecx,[g] ; jne exit+6`). No spelling of explicit calls (break, goto,
  explicit return) hoists. Use engine/CTimer.h with the inline bodies after the include on every script row with a timer.
- **The exit-copy registers follow the generated-script arm shape** (0x00D910B0). `if (!flag) { body } else { if
  (IsActiveThreadTerminating()) return; X(); return; }` gave retail's edx/eax per copy; `if (!terminating) X(); return;`
  swapped eax/edx in all three exit blocks.
- **A duplicated return in a loop exit reproduces a cold `fstp st(0) ; jmp` tail** (0x00A1AD40). `if (d > max) return
  C3DVector(...same as the final return...);` makes a separate tail that VC7.1 hoists from and tail-merges into the final
  return. It also fixed the prologue ebx push, the frame slot order and a hoisted member load. A `break` gives none of these.
- **Search-template stand-ins: take `&filter` AND pass the filter as a temporary** (0x008A8AF0). The address alone grew the
  frame by 8 (a named filter cannot share the slot a later local reuses); the temporary in the call shares it.
- **Respell to the PDB return types before fighting registers** (0x0051E630). `CThing::GetDefName()` returns a CDefString
  (Ego_r), not a CCharString; with `CDefString::GetString()` inline (named result) the copy construction is retail's. A
  user copy constructor on the 4-byte CDefString (or a named `long` local in the inline) then fixed an eax/ecx/edx rotation
  around the by-value-pushed table position.
- **The PDB C3DVector::operator+=(const C2DVector&)** (Ego_r ??YC3DVector@@QAEABV0@ABVC2DVector@@@Z) gives retail's
  `fadd [current.X]` step order where `current.X += step.X` loads current first (0x00A1AD40).
- **A named float local for a product fixes a trailing multiply order** (0x00A22CA0): `float angle = acos(d) * k; return
  angle * sign;` keeps `fmul k ; fmul sign`; the single expression multiplied by sign first.
- Ties confirmed again: ebx/edi between two webs (0x00956E70, even with the landed sibling's form), key-address temp
  rotation (0x006D4E60), the /Os float-slot push (0x0047ED76), SIB base/index (0x009E5C90), x87 operand order (0x00A22CA0,
  0x00A1AD40: a result-local return flips X/Z instead of Y/Z -- the choice depends on the whole function).

Review at landing:
- Grades per row (one lever each): 00a1ad40 11, 00d910b0 10, 009e5c90 9 (four of them a parallel batch of helper forms),
  0051e630 8 (match at the 5th; the rest name respells), 00712900 6, 006d4e60 5, 0047ed76 5, 00956e70 4, 008a8af0 4,
  00a22ca0 4. Rows above 6 were productive (each grade moved the residue) until the last two.
- Names to confirm: 0x004C7BC0 CThing::CheckUID() const (proposed, one source); the CWorldMap slot 10 ForceLoad(long)
  (unproven slot identity) in 008a8af0.
- Local models next to existing headers: 008a8af0 (CWorld, CWorldMap: the headers flatten the vptr); 00d910b0 (partial
  CQ_HobbeCaveScript / CScriptDef: no shared headers); 0051e630 (CThingBaseDef slot 10 only; engine/CDefClassBase.h not
  included).
- Already-landed sources with wrong spellings (NAME_FIXES.tsv): the landed GetUIScale 0x0041CF47 is non-const (PDB QBE);
  the landed sibling CAIStateGroupBriarRoseWillUse::Update 0x00956FE0 spells GetSubDef<CBriarRoseDef> as
  `?GetSubDef@CDefClassBase@@QAE_NPAUSDefOut@@@Z` and uses the `...Methods` derived stand-in class (the 00956e70 differ
  inherits both); several landed rows bind 0x00F35B30 as `@helper_f35b30@4`-style free functions.
