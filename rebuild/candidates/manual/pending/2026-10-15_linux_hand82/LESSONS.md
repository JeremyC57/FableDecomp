# Chunk 82 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `(float)x * 0.01f * member` makes VC7.1 move the literal to the end (`fild ; fmul [member] ; fmul [lit]`);
  `member * ((float)x * 0.01f)` keeps retail's `fild ; fmul [lit] ; fmul [member]` (0x007AEE40).
- A bool function testing a raw bit-field (`if (p && !p->Dead) return true; return false;`) materialises the
  bit (`mov dl,[ecx+91h] ; mov al,1 ; test al,dl`); reading it through an inline `bool IsDead() const` accessor
  gives retail's `test byte ptr [eax+91h],1 ; jne` + two exits (0x006BF960). In void/counting bodies the raw
  bit-field is fine (0x00781DF0, 0x007AEEC0).
- `return n > 0 ? n : 0;` compiles branch-free (`setle ; dec ; and`); `if (n > 0) return n; return 0;` gives
  the `test ; jg` over the shared `xor eax,eax` tail (0x006BF810).
- `member = std::min(member, ++arg)` with the Dinkumware const-reference min (`_Right < _Left ? _Right : _Left`)
  is the `inc ; mov [esp+4],eax ; lea eax,[esp+4] ; mov eax,[eax]` + self-store (`mov edx,[ecx+18h] ; mov [ecx+18h],edx`) shape (0x006BF860).
- A vector first pointer that is loaded before a call and reused after it (edi) while the index is re-read is a
  NAMED local (`const long* rents = def->Rent._First;`); a plain `def->Vec[i]` either computes the element address
  before the call or re-reads def/first after it (0x006C0270 matched; 0x006C0120 within one register choice).
- An empty tag temporary homed in a parameter slot that is still LIVE (`lea eax,[esp+8] ; push eax ; mov eax,[esp+0Ch] ; push eax`)
  reproduces when the call goes through an inline front end that takes the key by `const&` and builds the tag as
  its last argument (0x00747450); calling the out-of-line find directly reads the key first. This resolves part of
  the 24l "tag over a live slot" lane-gap note.
- SGI `find` front ends compiled /Gr: iterator CLASSES are never register arguments, so the hidden result takes ecx
  and the `const T&` value takes edx (passed through untouched); the 4-byte iterator needs a user constructor to be
  returned through the hidden pointer (a POD one comes back in eax: a 22-byte forwarder); with a user copy
  constructor each by-value iterator is built in its reserved slot (`push ecx ; mov eax,esp ; mov [eax],ecx`) and
  the tag's `lea` comes first (0x0052E0A0, 0x00535320, 0x00567A40).
- `return C3DVector(m.X, m.Y, m.Z);` (value constructor, arguments right to left) gives `fld Z ; fld Y ; mov ecx,X ;
  fstp [eax+4] ; mov [eax],ecx ; fstp [eax+8]`; `return m;` through the memberwise copy constructor is three integer
  moves (0x006E1B10).
- A static `__fastcall` returning C3DVector (user ctors) built in a named local `C3DVector v(p.X, p.Y, p.Z + c)` and
  returned through the memberwise copy constructor gives `fstp [local.Z] ; mov eax,[local.Z]` plus two direct
  `fstp` into the result (0x00878A10).
- A constructor copying a C3DVector field by field straight off `this` (`mov [esi],ecx ; mov [esi+4],edx`) stores
  three separate float members; a C3DVector member copied as a struct materialises `mov ecx,esi` first (0x00942230).
- `mov al,al` as the `true` of a `GetType() == 1` test is the same source as the `mov al,1` twins (VC7.1
  copy-propagates eax == 1); type id 0 gives `test eax,eax` (0x0087B7E0, 0x00737160, 0x00850780).
- The landed out-parameter `GetInterface(type, &out)` template also covers word-1 keys (type 51 ->
  `test [eax+24h],80000h`) and the second out-of-line LowerBound copy 0x004365B0 (0x0075D940, 0x0079CE80).
- Retail layouts confirmed by the bytes: CTCBoss PhaseStartPos +0x22 (unaligned), HelperCreatures +0x30, Effects
  +0x3c (12-byte vectors); CBossDef PhaseHealthPercent +0x28; CBuyableHouseDef Price +0x28, Rent +0x34,
  DaysBetweenRentPayments +0x40, MaxRentBags +0x44; CThing ThingTypeFlags +0x6c (bit 2 = physical, 0x40 = building),
  PhysicsTC +0x60, DefGlobalIndex +0x8c (ushort), Dead = +0x91 bit 0; CThingPhysical MaxHealth +0xb0, Health +0xb4;
  CTCPhysicsBase Position +0x0c, Radius +0x34, GetRHSet = slot 72.
- The CTCBulletTime ceiling (0.5f/1.0f stored as frame locals, `fistp dword` straight into a named local, integer
  compare of the float bits, `dec dword ptr [ebp-4]`, ebp frame) is inline `__asm` inlined into three members and one
  standalone copy: /QIfist gives `fistp qword` + copies, so it is a lane gap for byte-pure work.
- Explicit specialisation declarations of member templates (`template <> bool C::GetSubDef<X>(X*) const;`) crash the
  lane's cl.exe with C1001 (INTERNAL COMPILER ERROR): declare plain non-template members for out-of-line instances.

Review at landing:
- Rows that needed 4 grades (all parked): 006c0120 (GetPrice), 007e3040 (CTCCameraCollision::vfunc_4, one of them an
  ICE), 0075cd30 (GetEndPos). 00942230, 006e1b10, 006bf960, 007aee40, 006bf810, 0052e0a0, 00747450, 007aeec0 matched on
  the second grade.
- Local models next to existing engine headers: engine/CTCBoastingPosition.h, engine/CTCBulletTime.h and
  engine/CTCBuyableHouse.h exist but flatten the CTCBase vptr into `void* __vftable` and declare the intelligent and
  def pointers as opaque byte arrays, so the sources here use local models with the header's member names and
  offsets (a polymorphic CTCBase plus pointer classes with constructors/destructors are needed for the ctor/dtor/
  GetPItem shapes). The strict header check will flag them; retype only if the header grows real member types.
- Catalog rows whose label is wrong keep the catalog `Module::Name` on line 1 and pick the real symbol with
  `// Leaf:` (007aeec0, 00747450, 008ed010, 00a77540, 00942230, 0052e0a0, 00535320, 00567a40, 006e1b10); see NOTES.md.
- Already-landed naming inconsistencies noticed: CTCTimeAppearanceFade 0x007785E0 and CTCTavern 0x007BABC0 model their
  base as "CEngineSubPrimitive2DClockSprite" although the landed 0x00686800 source is CTCBase::CTCBase; the landed
  GetPointType instances 0x0072A430 / 0x0085A680 use a hand-built function-pointer vtable struct instead of a
  polymorphic class (bytes are fine; the model is not). 0x0077DB90 models 0x00686A80 as a `__cdecl` free function,
  while 0x006BF810 shows it is called as a `__thiscall` member (`mov ecx,esi` before the second call).
- No catalog row in this chunk holds several functions.

Coordinator note: parked 004fa9a0 / 0079c5f0 / 0079c680 / 0079c6d0 are one float-ceiling helper written as inline `__asm`
in the original (`fistp dword` into frame locals) -- not reproducible in plain C++ under byte purity; do not retry.
