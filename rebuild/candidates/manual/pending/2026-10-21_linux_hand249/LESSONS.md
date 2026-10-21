# Chunk 249 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00A87580 CSBGoal::Process: a named local in Process got its own frame slot (0x18 frame vs retail 0x10); moving it into an inline helper (`C2DVector GetOffsetToGoal(agent) { C2DVector pos = ...; return *Goal - pos; }`) lets VC share the slot with later block locals/temporaries.
- 0x0054BA20 CSlider::Update: VC7.1 reassociates `(v - min) * dim / range` into `(v - min) * (dim / range)` when dim and range are both memory operands; computing the product as its own statement (`offset = (v - min) * dim; return offset / range;` in a static inline helper) keeps retail's mul-then-div.
- 0x00851ED0 SetUpTransitionCamera: `fild ; fdivr [1.0f]` reciprocal CSE'd across two vector scalings is an inline `C3DVector::operator/(float d) { float inv = 1.0f / d; ... }` used twice; a hand-written `scale` local spills early.
- 0x00A88DE0 CQuaternion::SetFromRotationMatrix: `fld 0.0 ; fld st(1) ; fucompp` for `s != 0` comes from a double s (a float s gives `fld st(0) ; fld 0.0`).
- 0x009DDFF0 Draw2DTriangle: three vertex locals declared together land in reverse frame order; a `CTVertex v[3]` array gives retail's ascending order.
- 0x0041A980 CSprite::DrawWithRenderManager: `f()->a->b->Method(args)` pushes the args before calling f(); retail's call-first order needs `T* sys = f();` in a local. A two-vector C2DBoxF ctor computes both sums before storing; fieldwise box assignment stores each sum immediately.
- 0x0054B360 ~CSlider: an epsilon compare emitted as `test ah,41h ; jp` is `!(a == b)` with an inline operator== using `fabs(d) <= eps && ...`; `fabs(d) > eps || ...` gives `je`.
- 0x00EED8B0 CStopDrawing::Main: the CIsThingAlive filter needs its vptr (`virtual` member) or the vtable store vanishes; a `||`/`&&` chain of implicit CCharString conversions yields retail's `or ebx,N` construction flags.
- 0x00E63730 CSpectator::Init: `if (x) { if (IsDone()) return; A; return; } if (IsDone()) return; B;` arms let VC hoist the shared IsDone above the final branch.

- 0x00DA5F40 CRockTroll::Main: a function-scope named local packs into an earlier temporary's slot; giving it its own block reproduces retail's packing. A colour ctor assigning R, G, B, A in order groups the stores by value.
- 0x004B3260/0x004B35E0: the CCharString equality's length/Compare tail must be a nested inline (SameText) inside operator==; written flat, VC folds both results straight into bl.
- 0x004B6380 CQuestManager::LoadGameState: the `Mode == 2 || Mode == 3` test needs a named bool.
- 0x00904CD0 vs 0x00904540: GetInterface comes in two retail shapes. MoveToThing-family bodies bind the presence bit to a `const unsigned char& present = HasTC(t)` (byte spill into the shared key/control slot); WaitForEntityToFinishPerformingTasks uses the plain bitset test (`test ecx,ecx ; jns` for type 0x1F). Try both.
- 0x009041A0 / 0x006463FF: same-TU `#pragma auto_inline(off)` stand-in bodies (LowerBound, GetDef<T>, an event ctor, IS_THING_ALIVE) tell VC7.1 that address-taken locals do not escape. This fixes frame packing and the register/release forms.
- 0x0098A9A0 (coordinator idiom, confirmed): an STLport push_back's empty `__true_type`/tag temporary is homed over a LIVE local (here the dead `code` arg slot already holding a CCharString) only when the growth insert (0x00440990) has a same-TU noinline body that never reads the tag. Declared-only, the tag claims the first free arg slot, which costs a 4-byte frame slot. The same trick moved 0x00BECDE0's overflow tag into the arg slot.
- 0x0098A9A0 / 0x004B1BA0: the counted-pointer release `mov r,[eax+N] ; mov ecx,eax ; add eax,N ; dec r ; mov edx,r ; test/cmp edx ; mov [eax],r ; jne` is the landed SetQuestInfoText ReleaseTemporary idiom: `DecRef(); if (GetRefCount() == 0) Destroy()/delete this;` as a method on the pointee, called as `if (Data) Data->ReleaseTemporary();`.
- 0x004B1BA0: `g_pEngine->Bank->GetTagFromCRC(s.GetCRC())` loads the bank after the Calc call. Retail loads it between the CRC arguments and the call, which comes from an inline getter FUNCTION (`GetGameTextBank()->...`). A conditional temporary's first dword is zeroed at function entry automatically.
- 0x00F3A710: a strength-reduced bit loop (`mov eax,1 ; add eax,eax`) is an explicit mask variable advanced with `*= 2`; `<<= 1` gives `shl r,1` and `1 << b` gives `shl cl`. The dl/bl/frame split of several bools follows their declaration order (permute it).
- 0x004B4260: a run of bools/ints in the same frame is register-allocated by VC's own weighting; declaration order, continue forms and noinline callee bodies did not move it (parked).

Review at landing:
- Local base models sit next to existing engine headers. These need a header decision:
  - CQuestManager rows 004b1ba0/004b3260/004b35e0/004b6380 (retail is 0x5C lower than engine/CQuestManager.h)
  - CRegion 006bbfd0 (retail 0x58 model)
  - CSprite 0041a980
  - CRegionMinimap differ 0066c900 uses the header
- Same-TU stand-in bodies live in the TUs of 009041a0, 006463ff and 0098a9a0 (growth insert 0x00440990). They are noinline oracles of other retail functions, not reconstructions of them.
- 0098a9a0's test is the only one that needs `#define __stdcall` for g++ (COM slot pointer).
- 004ca230 lands under CThing::CreateTCsFromString, not its catalog label.
- 00becde0's class identity (CVectorMap member, not CRandomAppearanceMorph) needs settling before it lands.
