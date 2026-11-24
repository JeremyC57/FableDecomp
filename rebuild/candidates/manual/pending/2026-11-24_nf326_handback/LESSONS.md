# NF326 lessons (2026-11-24, the CScriptThing pick-any unit)
- Census by object, not text: compile every candidate at HEAD and read the COFF symbol table WITH the COMDAT selection (section aux record
  byte 14: 1 = no-duplicates, i.e. strong; 2 = pick-any). Text missed 27 local-model copies (`class CScriptThing` / `struct CScriptThing` in the row, found first as three, then
  by an in-class scan of every row that models the class) and two text copies emit nothing. tools/objcensus.py does both columns.
- Why copies are emitted at all: a TU that defines CScriptThing's ctor / dtor inline emits `??_7CScriptThing@@6B@`, and with it every inline
  virtual of the class as a pick-any COMDAT, used or not. Check the vftable column before deciding a copy is "only an inline expansion".
- (b) is answered by the exe: count the copy rows among the retail CALLERS of the self address (tools/census_addr.py). 0 of 86 for GetPos.
- A strong self + pick-any copies is LNK2005 under a real link (LINK= cleared, /FORCE:UNRESOLVED; plain /FORCE hides it as a warning, and
  /OPT:REF in a /NOENTRY DLL discards every unreferenced COMDAT so the map shows nothing: add /OPT:NOREF). The ~CScriptThing self already
  links that way today.
- Pick-any is only safe when every emitted copy equals retail: compare each copy COMDAT with Fable.exe at the self address, relocation
  fields zeroed and relocation symbols listed (tools/copycmp.py). 116 of 164 differed; the ones that matched shared one spelling
  (`PImp.IsNull()` / `PImp->`), a strong hint at the original source.
- Inline accessor calls inside an emitted inline (PImp.IsNull(), PImp->) make VC7.1 emit the accessors themselves as unreferenced pick-any
  COMDATs: same bytes, but the DEF set grows (NF320 Q2). Read the DEF set after every respell, not only the grade.
- A body respell of an inline the row expands can flip that row's own leaf (register / lea operand order) even with identical out-of-line
  bytes: grade every copy row, HEAD and new at ONE work path (tools/pairgrade.py), and keep a second spelling ready (the parenthesised float
  return fixed GetAngleXY in two rows).
- Compiler-internal `$L<n>` labels (switch tables) renumber when an inline body adds labels: compare their relocations by section + value.
- `awk -v a=00e36390` compares numerically (0e36390 = 0): look rows up with python or `-F'\t' '$1==a""'`.
- nf.py's lock owner and agent path are hard-coded: sed both after copying the tools (they were NF323's).
- The repo moves under a long run: before handing back, list the headers changed since the run started (`find -newer`), walk each changed
  row's include closure, and regrade the rows whose closure holds a changed header (here engine/CGameScriptInterface.h: 34 rows graded before
  it changed were regraded, objects equal to the first run's in every non-debug section).
- Hand-written in-class models vary (PImp.Data / PImp_Data, a local counted-pointer struct, a pointee spelled CScriptThingImp / Impl):
  parse the model's pointee type, data expression, callee and null object instead of templating them (tools/respell_inclass.py), and let
  clang show the type when the regex guesses wrong (0x00D73C70).
