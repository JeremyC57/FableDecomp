# NF328 lessons (2026-11-24)
- A ctor / dtor has no address: emit it pick-any with a call instead. `thing.CScriptThing::~CScriptThing()` (qualified, so non-virtual)
  and `CScriptThing f(const CScriptThing& t) { return t; }`, each under `#pragma inline_depth(0)`, add nothing but the helper.
- An `inline` destructor that nothing references is not compiled, so the TU loses the class vftable and its ??_G too. A ??_G row needs a
  reference (the helper) once its strong destructor becomes inline. With the helper, VC7.1 still expands the body into ??_G.
- A user dtor that releases members IN ITS BODY lets VC7.1 hoist the first member load above the vftable store. Retail's order (vftable
  store first) needs the release in a MEMBER's destructor. Any member type with a destructor is then also emitted pick-any (.debug$F
  reference), even FABLE_FORCEINLINE ones.
- Count the census from compiled objects, not from the queue line: the full text gate (dtor / copy bodies, local models, implicit-copy
  opt-ins) was 6x NF326's set and found 5 more strong definers.
- Never print relocation targets decoded from retail at a DIFFERING copy's offsets: they are retail bytes, not addresses.
- A full /FORCE link of ~1,000 objects with /MAP runs in well under a minute; the duplicate-line delta per symbol is the clearest proof
  that a one-definition change removes exactly the intended LNK2005s.
