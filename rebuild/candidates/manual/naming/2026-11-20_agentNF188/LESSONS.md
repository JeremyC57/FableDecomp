# NF188 lessons (one line each)
- A row-local stream model is usually just `class CMemoryDataInputStream { ...; virtual ~...(); }`: dropping it for engine/CMemoryDataInputStream.h
  moves the destructor binding to ??1CDataStream with no other change (17 rows, one grade each); script it (tools/mds.py, mds2.py).
- When a transformer replaces two adjacent classes, a placeholder without its newline glues the next `class` to the previous line and
  the next `^class` regex silently misses it.
- Member names of the row-local stream models vary (ReadULong / ReadLong / ReadFLOAT / ReadData / ReadToStream / GetDataAndSkip / StreamSize):
  map them to the header's inline names (ReadULONG / ReadSLONG / ReadFloat / ReadCompressed / CopyTo / ReadConstData / GetLength()).
- The shared ReadConstData is plain inline; a large caller may need a per-row /d2inlT flag to expand it (0x00BDE860: 300).
- mkpay.py ignores a row's `// Flags:` line: a vland payload of a Flags-line row grades DIFFER unless the flags are put in (tools/mkpay2.py).
- binds.py needs the row's real leaf: for rows without a Leaf line pass the object's defined symbol, or the HEAD/new pair diff is empty.
- A test that links the row object needs stubs for every out-of-line virtual of a shared polymorphic class it now uses (CBaseClass too).
- VC7.1 ICEs on a call through an explicit-specialization declaration in a test; leave the primary template declaration-only instead.
- Shared headers can carry #error guards (CExpression_FishScript.h needs FABLE_RETAIL_NSCRIPT): the cpp precheck catches them before cl.
- A RELOCATION_MATCH port can still bake a stand-in into a shared header through a return or parameter type: check every class token an
  opt-in declaration names against its own header / PDB size before handing it back (q5108: the CSBAgents token was a row-local guess).
- reloc_bindings census lines are table evidence only (one witness per symbol, can be stale): probe the binders at HEAD before "done-already".
