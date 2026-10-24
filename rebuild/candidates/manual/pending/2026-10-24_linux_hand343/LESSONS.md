# Chunk 343 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- MSVC lays overloaded virtual functions out in REVERSE declaration order inside their vtable group. Examples: CText
  `SetText(CCharString)` slot 145 / `SetText(CWideString)` 144 (0x557AF0); CIconText SetValue 145/144 (0x541E60/0x541F20);
  CList ScrollUp(5 args) 165 / ScrollUp(bool,bool) 166 (0x53605F); SetDefaultChild(CComponent*) 157 / (long) 158. Declare the
  overloads next to each other, in the opposite order to the slots you want.
- A virtual found only in the second vtable (CObserver at +4, e.g. ProcessEvent slot 1) compiles with `this` = the subobject,
  and every member is read at -4. Model it as `struct X : CComponent, CObserver` and override it in the class (0x54DBC0, 0x54E280).
  Retail CObserver is 0x14 bytes: CClickable's third vptr is at +0x18.
- Retail `if (Def()->Flag) ...` against a CDefPointer temporary reads the flag into bl before the inline release. A call that
  takes a definition field as its argument (`ChangeState(Def()->State)`) runs BEFORE the release, because the temporary dies at
  the end of the full expression (0x55AFD0).
- A C2DVector returned by value whose X is copied through the FPU (`fld/fstp`) and Y through a register needs a USER copy ctor
  `C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}`. The implicit copy moves both halves through integer registers (0x53CDCA).
- A setter `m = p;` over CCountedPointer that compiles to `call ShareData ; ret 4` needs a `void operator=`. A `T& operator=`
  returning `*this` keeps `this` in esi and ends with `mov eax,esi` (0x53CE97).
- Two virtual calls that differ only in their argument (`if (..) c->F(x); else c->F(1);`) are tail-merged into
  `push 1 / push edx ; call`. A ternary argument computes the value into a register first (0x5361D3).
- STLport `list::push_back` = `insert(end(), x)`: the position is read before `malloc`. Binding `prev = pos->prev` to a local
  avoids a reload after the `tmp->next` store. A `throw()` placement new gives the `lea ecx,[eax+8] ; test ecx,ecx` null test (0x5586A0).
- A short-lived local can reuse the frame slot of an earlier one (string tag, then CRGBColour) only when both sit in SIBLING
  blocks. A local in the enclosing block opens a second slot (`sub esp,8`) (0x557AF0).
- The `this`→second-base conversion keeps its null test when passed directly (`AddObserver(this)`, 0x557C10). After earlier
  calls through the adjusted pointer, VC passes the already-adjusted esi with no test (0x557AF0).

Review at landing:
- Local base models sit next to existing engine headers. CObserver.h, CHoverable.h, CList.h, CFrontEndList.h,
  CFrontEndScreen.h and CManager.h are flattened donor layouts (`void* __vftable`, PDB offsets) that cannot carry the virtual
  dispatch, and their offsets disagree with retail (e.g. CManager RightStickForce donor +0xc0 vs retail +0x94).
- Landed 0x00557BD0 CKeyRedefiner::CancelSelection names 0x0041E5F2 `CFrontEndManager::GetInstance` and its slot 4
  `SetFocusControl`. They are CManager::GetInstance and CObservable::RemoveExclusiveObserver.
- Landed 0x00992470 is labelled CFrontEndScreen::AddActionOnLeftClicked (member +0x170, in no vtable). The real
  CFrontEndScreen::AddActionOnLeftClicked is 0x0054E4F0 (vtable 0x012497E4 slot 71, member +0x160). Give 0x00992470 another
  class name, or the two symbols collide.
- The name of 0x00543B10 (CIconText slot 151, GetTextAlignment) is descriptive: Ego_r.pdb has no out-of-line body for that slot.
- None of the 40 rows needed more than 4 grades. No row holds several functions.
