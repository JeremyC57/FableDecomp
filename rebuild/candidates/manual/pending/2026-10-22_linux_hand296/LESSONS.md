# Chunk 296 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A RELOCATION_MATCH can hide a wrong vtable.** A `__declspec(novtable)` model can store the wrong `??_7` in an inlined
  ctor and still grade RELOCATION_MATCH. A forceinline holder can match bytes only through a shadow `??1` call. Run
  `probe/rpair.py` and refuse any row whose vtable or dtor symbol pairs to the wrong target (00e1cae0 refused,
  00e1cd40 verified).
- **Same-TU `#pragma auto_inline(off)` stand-ins carry interprocedural facts in VC7.1.** An ungraded out-of-line
  body for a callee in the same TU (clear, GetAcceptedBoastsForScript, list::remove) tells the compiler what the
  callee does with its arguments:
  - a local list's header stays in a register across the calls (PostBoastTransfer);
  - after `param.Reset()`, a by-value CCountedPointer parameter's zero stores and end-of-function destructor fold
    away, and its dead slots host later temporaries (DeactivateQuest).
- **The message zero vector.** `World->m_pMessages->AddMessage(type, dist, C3DVector(0,0,0), ...)` builds the zero
  C3DVector after the distance push only when the call is wrapped in an inline (forceinline if needed) CQuestManager
  helper. Calling it directly builds the vector early.
- **Read ID fields into locals before a by-value CCharString argument copy** (`long id = node->value.ID;
  GetBoast(id, node->value.ScriptName)`). Otherwise the copy is ordered first.
- **`bool b = true; if (reason & 2) b = false;`** gives retail's byte-slot bool. `!(reason & 2)` gives shr/not.
- **The STLport vector dtor caches finish via a by-value `DestroyStrings(start, finish)` helper.** Without it, the
  loop re-reads `_M_finish` (+2 bytes, ~CQuestManager).
- **An inlined `_M_insert_overflow` at only one of two push_back sites needs a `__forceinline` copy of the overflow.**
  The out-of-line template body otherwise stays a call at both sites.
- **CDataInputStream::Read into a shared slot.** When retail reads two longs into one frame word and keeps the first
  in a register with no store, wrap the read in `__forceinline long ReadLong() { long v; Read(&v, 4); return v; }`.
  Separate named locals grow the frame. One reused local keeps a dead store and also spills the count, which costs
  the `cmp edi,edi` zero reuse for the 64-bit high dword (00bde390).
- **Size-optimised NPlayerGui TU (0x0064xxxx-0x0065xxxx):**
  - `#pragma optimize("s", on)` alone gives `mov ebp,esp` frames here (frames over 0x80).
  - Literal `L"..."` needs `const unsigned short*` parameters (no /Zc:wchar_t).
  - A retail sprite box copied from a temp means two box types: an out-of-line C2DBoxF(pos, size) 0x0041A7E0, then
    an inline converting ctor into the sprite's box parameter.
  - A colour whose common channels are stored before a branch and the rest per branch comes from a forceinline
    `if (...) return CRGBColour(...); return CRGBColour(...);` helper. A ternary argument gives two temps, and a
    copy-initialised local adds a copy.
- **A temporary in a Draw2DText argument list is constructed between the pushes.** When retail constructs the
  CWideString before any push and destroys it straight after the call, use a named local in an inner block.
- **The empty iterator tag can be homed in a live argument slot.** VC7.1 put `__find`'s empty
  random_access_iterator_tag in the live `reason` argument slot (DeactivateQuest). Five source shapes failed to
  reproduce it; treat it as unreachable.
- **Earlier in this chunk:**
  - forceinline `operator=` for CScriptThing when retail inlines the counted-pointer assign;
  - the polymorphic CBaseObject model (`this` passed at +0);
  - a `#pragma inline_depth(0)` region from a success block past the closing brace (WardenKeyMiniGame);
  - `for(;;) { if (term) return/break; ... }` to place shared-free tails.

Review at landing:
- 00e1cd40 matched with the novtable + forceinline-dtor model. rpair shows the correct vtable and dtor symbols, but a
  reviewer should confirm that no `??_7` base store is expected in the inlined ctor.
- 00bde390's `ReadLong` helper is a reconstruction convenience; check whether the landed CDataInputStream header
  already has an equivalent templated `>>`/Read<T> to reuse.
- The NOTES label corrections (00ec1910, 00e75ba0, 00dd23a0, 00dd9520, 00ee76f0, 0x0049D870, 0x004BA390, 0x004B8770,
  0x009F0500, 0x00BDFE70, 0x0041A7E0) should go into a labels_chunk296_corrections TSV.
- The six one-pair/one-slot near misses (006509f8, 004b3a20, 004b2890, 00dd9520, 00e06050, 004b2280) are good
  candidates for an advisor pass.
