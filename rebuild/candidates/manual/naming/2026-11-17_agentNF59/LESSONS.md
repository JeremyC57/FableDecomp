# NF59 lessons (2026-11-17)

## Recipes on the shared headers

**Protected or private STLport members.** An explicit instantiation ignores access, so `template void std::vector<T>::_M_clear();` and `template void map<..>::_Rep_type::_M_erase(node*);` emit protected or private members on the shared headers. No local `std` model is needed. Name the tree through `map<K,V>::_Rep_type`.

**Uncalled `??_G` or implicit `~map` COMDATs.** A one-line helper that `delete`s the object or calls `->~map()` makes VC7.1 emit the COMDAT. This is the landed 0x006F24D0 pattern. Mark the helper as not retail.

**GFSerialiseVectorBinaryIn<T> when retail calls `resize(n, x)` out of line:**
- spell the template body with `resize(count, T())` and use FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE;
- the header body's one-argument `resize(n)` stays an out-of-line call under /Os.

**Traits frame homing.** When retail homes an empty traits object over a dead argument slot, keep the callee's body visible but not inlined. Write it as `inline __declspec(noinline)`, which makes it a COMDAT with no strong-definition clash.

**CPersistTraits<bool>.** Retail has an explicit class specialisation. Use FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS for both GFSerialise<bool> rows; Out folds the traits away and In keeps the call.

**Gossip script removers.** CGameScriptInterface::RemoveRumourCategory / RemoveGossipVillages / RemoveGossipFactions port on NF57's 0x008AA050 include set in one grade each. Each one is a key copy, then `CTCAIScratchpad::<map>.erase(key)`.

**The CTCPhysicsBase retail vtable already names the virtuals.** CanCollideWithThings, CanCollideWithThingsOneWay, CanInteractWithThings and IsExcludingCreatures are there; local models had made up "GetCollidesWithThings" names. CThingSearchTools needs `FABLE_RETAIL_CTHINGSEARCHTOOLS` before its template declarations appear.

**Include order:**
- rebuild_abi.h must come before stlport/fable_containers.h when the DISPATCH_FORCEINLINE path pulls in fable_string.h; otherwise the build fails with an undefined fable_i32.
- engine/CPersistTraits.h needs engine/CDataOutputStream.h (FABLE_RETAIL_CDATAOUTPUTSTREAM) first.

## Avoiding slips

**Rewritten tests must print the catalogue PassPattern**, not a new pass string. Read the pattern in build_candidates.ps1 before rewriting a test. Note that some patterns name the old stand-in.

**Never read a file inside a `{ ...; } > same-file` group.** The redirect truncates the file first. Assemble the new row in work/, then move it into place.

**Quote every argument that contains `<` or `>`** (template names), or the shell treats it as a redirection.

**Check the helper's usage line before scripting it.** st.py takes `<addr> <change> <grade> <test> <grades> <note>`; an extra entry-id argument silently replaces the note.

**When two retail names give identical bytes and there are no callers** (~map vs ~_Rb_tree), use neighbour COMDATs of the same class as the tie-break, and record the alternative for the coordinator.
