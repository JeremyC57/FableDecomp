# Chunk 99 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- C3DVector copies have three distinct shapes: a PLAIN POD assignment (`m = v;`) copies through a `lea reg,[this+N]` base and a
  carrier register (0x00727120, 0x00726920, 0x00759A00, 0x00723930); a by-value return of a member through a MEMBERWISE user
  copy constructor interleaves `mov edx,[ecx+N] ; mov eax,[esp+4] ; mov [eax],edx` with the result left in eax (0x00723C00,
  0x00759B20; the POD return is an esi block copy); a user copy constructor over FLOAT members copying a frame local sends X
  through the x87 and Y/Z through integer moves (0x006E7FD0).
- The lerp `return (b - a) * t + a;` over three inline value-constructing C3DVector operators reproduces the 0x18-frame x87
  shape (difference Z spilled, scaled X/Y spilled, `fld ; fadd` into the hidden result) at first try (0x0049D150, 0x004C22C0,
  0x004C2EF0, 0x00AFE320); the variant that spills the SUM's X and ends `fld [esp+0Ch] ; fstp [eax] ; fxch ; fstp ; fstp` is the
  same expression into a NAMED local returned through the memberwise copy constructor (0x00723C20).
- `call A ; call B` with no `mov ecx,eax` between and B ignoring ecx = a STATIC member called through the object expression
  (`GetWorld()->GetFrameNumber()`): the object is still evaluated, its result dropped (0x00723930, 0x006AF740).
- A polymorphic class that DECLARES an 8-byte member (`unsigned __int64`) gets 4 padding bytes after its vfptr (data from +8);
  retail's CTCBase data at +4 needs the vptr and the CTCBase words in a base class (0x00800500 graded `lea esi,[edi+10h]` until
  the base was split out).
- `bool` bitfields: `return a | b;` (bitwise) of two one-bit fields gives `movzx ; mov ; shr 1 ; or ; shr 4 ; and 1` with no
  branch (0x00724C50); two bitfield stores of the same byte (`x = arg; y = true;`) merge into ONE store (`shl ; xor ; and ; xor ;
  or 1`, 0x00727120); `bit1 = bit0` copies give `mov dl,al ; shl dl,1 ; xor ; and 2 ; xor` (0x006AF740).
- A fill-the-caller's-record function whose eax holds the record at `ret 4` and whose stores follow no member order is
  `T& Get(T& out)` with the statements in the retail store order (0x00C442F0); returning a named local by value loads every
  word into callee-saved registers first.
- `if (a && b) return 1; return 0;` with a `long` result keeps the two `mov eax,1 ; ret` / `xor eax,eax ; ret` exits
  (0x00639B70, 0x00800500); a pointer-literal argument to a CWideString compare is the pooled `L""` (0x0122D70C).
- A `float` member initialised with the INTEGER literal `0` in an init list shares the integer zero register (0x006398B0,
  `DisplayTime(0)`).
- CCountedPointer::operator= whose source OBJECT load precedes the destination block load (`mov ebp,[ebx+N] ; mov eax,[esi+N+4]
  ; mov edi,[ebx+N+4]`) reads the object into its local BEFORE the block (0x00728400, 0x00AF6690); the landed 0x0084F130 order
  (block first) is the other schedule.

Review at landing:
- Rows that needed more than 4 grades: 00728470 got a fifth grade only to add `__forceinline` to a probe whose inline
  GetInterface was emitted out of line (it then graded the same 71v71 register diff); parked.
- Local base models next to an existing engine header: CTCPhysicsStandard / CTCPhysicsBase / CTCPhysicsNavigator /
  CTCPhysicsRigidBody / CTCTrapBase rows model the classes locally (the headers are flattened PODs without virtuals; the
  CTCPhysicsRigidBody.h and CTCTrapBase.h offsets follow the PDB's 16-byte vectors, 4 bytes high per vector in retail). Only
  00702560 includes engine/CTCQuestCard.h (inline accessor viewing RegionName_PStringData as a CCharString).
- Headers that disagree with retail: engine/CTCPhysicsRigidBody.h (RHSet at +0x74 in the header, +0x68 in retail),
  engine/CTCTrapBase.h (0x6c in the header, 0x60 in retail), CTCPreCalculatedNavigationRoute / CTCQuickAccessMenu /
  CTCScriptedControl / CTCRandomAppearanceMorph PDB layouts (12-byte vectors, 4-byte lists).
- Already-landed sources: rebuild/src/compiled/00/72/CTCPhysicsStandard_ResetInterpolation_007236e0.cpp models RHSet /
  OldRHSet as `Xform6` with a pad struct and three free fastcall callees (no class header, no names), and
  CTCPhysicsNavigator_SetPosition_00726750.cpp writes through raw offsets (`*(Vec3*)((char*)self + 0x89)`) -- both could be
  retyped onto the chunk-99 local model.
- Catalog rows holding several functions: none in this chunk. Note 007238E0 / 007239C0 are byte-identical bodies in two
  vtable slots (74 / 75).
