# Chunk 437 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A register that survives a call to a tiny out-of-line callee (edx = `this` across the visitor constructors
  0x00C36270 / 0x00C36DA0 in 0x00C362F0 / 0x00C37000) is NOT an LTCG gap when retail's TU holds the callee: a same-TU
  `__declspec(noinline)` body of the callee reproduces it. Do NOT put that body under `#pragma auto_inline(off)` when
  the callee has an inline base constructor: the base constructor then becomes a call, the stand-in clobbers edx and
  VC7.1 moves `this` to esi (78v75). ltcg_grade.py does not help here (internal: an esi convention; exported: esi).
- A true register ARGUMENT (esi used unset as the first instruction's operand, 0x00C30E90) stays a lane gap: one
  reference grade, park.
- Inline find-or-create with an early return (`if (E* e = Find(k)) return e; return Create(k);`) merges both results
  in eax before `mov esi,eax` (0x00C31230); `E* e = Find(k); if (!e) e = Create(k);` (inline or open-coded) tests esi
  and is 2 bytes longer.
- A frame 4 bytes larger than the model with one byte read back from its bottom slot: the "flag" is the first field of
  a record the callee fills (0x00C32060: an 8-byte {bool, message} reply copied out by the handler), not a bool local.
- A local whose inline constructor makes two out-of-line member-constructor calls gets its single EH state after BOTH
  calls (0x00C32060: handler 0x00C1DD50 then mailbox 0x00C1D9D0, then state 0 for the composite's destructor).
- A float argument pushed after an integer argument through `fstp [esp] ; mov ecx,[esp] ; push eax ; push ecx` (the
  push-ecx slot) is a named float local (`float v = a * b; f(v, mask)`); written inline in the call it is
  `push ecx ; fstp [esp]` (0x00C34410).
- `if (GetY() == y && GetX() == x) return false; ... return true;` gives two `test ah,44h ; jp <store>` with the false
  return on the fall-through (0x00C34530, first grade).
- Several identical early `return -1`s put the merged failure block on the fall-through of the last test and the
  success code at the end (0x00C34630); nested ifs put the success on the fall-through.
- An ecx + edx entry with both registers consumed and `ret 8` for two stack args is a legitimate `__fastcall` free
  function (0x00C34630: `__fastcall(format, source, dest, size)`; the prologue matched first time).
- Retail self-copies of a member (`mov cx,[esi+0Ch] ; mov [esi+0Ch],cx`) plus no load hoisted over an earlier store to
  the same record are the volatile signature, like a dead re-test (0x00C34630, 0x00C35DD0 -- both libpk sample/format
  records, probably shared with the mixer thread): park after 1-2 grades as "volatile candidate".
- A polymorphic local's inline (virtual, empty) destructor costs nothing at the normal scope exit, but still gives the
  EH state and the unwind call to the out-of-line copy (0x00C31F60 base 0x00C31E80, 0x00C362F0 0x00C36690).
- x87: a 1/x reciprocal or a call result that retail keeps in st0 with `fst` (and multiplies as `fld kept ; fmul mem`)
  is spilled with `fstp` + reload by this lane whatever the source operand order (0x00C36B90, 0x00C34440) -- park after
  2 grades as x87 operand order. The call ORDER (floor(y) before floor(x)) does follow the source statement order.
- Earlier in the chunk (same session): `cond ? 1 : 0` on a bool -> `neg al ; sbb eax,eax ; neg eax`, `ok ? &g : 0` ->
  `neg/sbb/and` (0x00C335C0, 0x00C334D0); inline `(m_flags >> n) & 1` -> `shr edx,n ; test dl,1`, 1-bit fields ->
  `test al,4` (0x00C35A10); strncmp must be a `__declspec(dllimport)` declaration (IAT call) and argument order matters
  (0x00C2ECD0); a void member ending in `if (!IsOk()) return;` -> tail `jmp IsOk` without `xor al,al` (0x00C2EB00); an
  `unsigned char` result -> `sete al` with no `xor eax,eax` (0x00C31D40); a ushort member widened through an
  `unsigned long` parameter -> `movzx` (an `unsigned short` parameter -> `xor ecx,ecx ; mov cx`) (0x00C2EAB0); a
  discarded `new T(this, x)` in a void member -> one epilogue (0x00C36180); destructor loops
  `for (count = m_count; count; count = m_count) Remove(data[count - 1]);` (0x00C34B80); the "$E2" row 0x00C33770 is a
  plain function over extern globals (an inline-constructed static folds into .data).

Review at landing:
- 0x00C362F0 used all 4 grades (q.py 78v75, two ltcg_grade.py runs, q.py RELOCATION_MATCH) plus one re-verify after a
  rename-only rewrite; 0x00C336F0, 0x00C35990 and 0x00C35DD0 also used 4.
- Same-TU stand-in bodies ship in two landed sources (0x00C362F0 CAllocateVisitor ctor, 0x00C37000 CMaxValueVisitor ctor);
  they are the retail TU's own bodies (byte-identical shapes to the landed 0x00C36270 / 0x00C36DA0).
- No engine headers exist for any class used here (libpk); all models are local.
- Already-landed sources with wrong names (bytes fine):
  - 0x00C0C200 "CShaderResource::GetPixelShader" is CString::operator=(const char*) (AssignCopy(SafeStrlen(p), p); returns this).
  - 0x00C3B900 "CLandscapeLayerMesh::CLandscapeLayerMesh" is the 0x20-byte PKCAutoMap wave entry constructor (key at +0, the
    empty CString data at +4).
  - 0x00C31F00 "CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch" is CMemoryStream's destructor (+4 buffer, +8 cursor --
    the landed "m_textureCount" is the cursor pointer).
  - 0x00C1DD50 "InitStateGroup" is the PKCOneShotQueue reply handler's constructor (vtable 0x012B332C).
  - 0x00C1D9D0 "CScriptEntry ctor" (manifest _Dest_val) is the CMailbox constructor (handler at +0x18, event at +0x1C);
    0x00C1DCE0 "CPair dtor" is the sync-wait object's destructor (mailbox at +0x10).
  - 0x00C20340 "CDragSource::BeginDrag(long)" returns a float distance in st0 (tail call to the vector length 0x00C25970);
    the landed `void` model is byte-correct but its callers (0x00C3448E) use the result.
  - 0x00C36270 "CVtable_012b50cc ctor" and 0x00C36DA0 "CBaseIntelligentPointer::CBaseIntelligentPointer" are the visitor
    constructors of 0x00C362F0 / 0x00C37000 (the latter's `m_pItem = 0` is the float max = 0).
  - 0x00C1BD60 is labelled `__EH_epilog3` (a CRT helper name) but is a plain setter (`[ecx] = arg`), folded with the
    0x00C1BD40/50/70 float-pair accessors used by 0x00C34530.
  - agent436's CSegmentFile::OpenSegment(unsigned long id) 0x00C2D9C0: callers (0x00C30ED3) pass the address of a CString
    name, so the parameter is a `const CString&` (byte-identical).
