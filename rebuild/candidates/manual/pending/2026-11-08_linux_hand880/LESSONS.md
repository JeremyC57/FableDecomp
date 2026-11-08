# Chunk 880 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x0058AF10) A pointer local that retail keeps in a callee-saved register while every assignment also stores it to a frame
  slot (`xor edi,edi ; mov [ebp+10h],edi` ... `lea eax,[ebp+10h] ; push eax ; call vector::push_back`) is a plain local passed to
  the INLINE STLport `push_back(const T&)` body that VC7.1 declined: the reference temporary is coalesced with the local's home.
  The out-of-line push_back declaration opt-in address-exposes the local instead (memory only, larger frame) -- use it only when
  the row's other locals are not affected.
- (0x0058AF10) A jump table that starts at 0 with entries 0 and 1 going to default needs an explicit `case 0:` with its own body
  (identical to default, cross-jumped); `case 0: default:` on one label drops case 0 and the table starts at the next case
  (`add eax,-2 ; cmp eax,0Eh`).
- (0x0058AF10) `CText* t = static_cast<CText*>(p.Get()); if (t)` stores the spilled pointer before the branch;
  `if (!p.IsNull()) { CText* t = static_cast<CText*>(p.Get()); ...` stores it after (retail).
- (0x0058AF10) `obj->Set(CRGBColour(0,0,0,0))` fetches the vtable before the temporary's byte stores; a named
  `CRGBColour c(0,0,0,0); obj->Set(c);` builds it first (`lea ecx,[c] ; mov byte ptr [c+2],0 ...` then `mov eax,[esi] ; push ecx`).
- (0x0058AF10) `static_cast<X*>(p->FindChild(n))->Set(arg)` pushes arg before the FindChild call; a named `X* x = ...; x->Set(arg);`
  pushes it after (retail).
- (0x0058AF10) `frame->Add(*it)` folds to `push [ebx+8]`; `CComponent* picture = *it; frame->Add(picture);` gives retail's
  `mov eax,[ebx+8] ; mov edx,[esi] ; push eax`.
- (0x0058AF10) Two iterators walked in step: their frame slots follow the declaration / increment order (declare the one retail
  stores first before the `for`, increment it first); null tests on values read through them as nested ifs on named locals keep
  the loads lazy and the non-null facts (no `neg ; sbb ; and` on a later base-class conversion).
- (0x0058AF10) STLport 4.6 `make_pair("A", "B")` needs the `_STLP_FUNCTION_TMPL_PARTIAL_ORDER` array overloads (now behind
  FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS); the shared header's single `make_pair(const T1&, const T2&)` fails with C2536 on literals.
- Method: with 849 calls in the row, aligning the retail call sequence with the object's DISP32 relocations (position by position)
  proved the call structure identical after the first draft; then a diff that masks only frame offsets / relocations isolated the
  ~10 real residues, and a slot map (retail offset -> built offset over equal instructions) showed which locals' frame slots moved.

Review at landing:
- 17 grades (more than 4): all logged in ATTEMPTS.log; the final source is v16 with comments (src/0058af10.cpp).
- NAME_FIXES.tsv: 0x00592495 landed as pair<const CCharString, CCharString> (its comment calls 0x00593ADE a map insert; it is a
  list insert).
- Header overlay (hdr/README.md): the stlport change is in fable_containers.h; the row includes `stlport/fable_containers.h`
  first only so the overlay copy beats the repo copy (quoted includes resolve next to the includer).
- Ego_r.pdb CRedefinerList::PAcceptButton is +0x1BC; retail writes +0x18C (STLport shrink) -- the shared header is not used for
  that member here (SetAcceptButton is called out of line).
