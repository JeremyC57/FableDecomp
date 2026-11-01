# Chunk 691 lessons
- Quest-script thread functions (CSpawnedFunc targets) are named by the string passed beside the member-function pointer store
  (`mov [edi+34h], <fn>` after the CSpawnedFunc base ctor): the string is the PDB method name.
- A local STLport vector whose finish pointer retail keeps in esi/edi across calls (and whose first push folds to position 0) needs
  same-TU auto_inline(off) bodies for every member that receives the vector's address (_M_insert_overflow, operator=); /Oa does nothing.
- `X() + L"..."` where retail pushes the right operand before building X: X is a temporary written in place, not an inline function
  returning by value (the inline call is evaluated before the push).
- engine/CDefPointer.h pulls <stddef.h>, which hangs cl under q.py; model CDefPointer<const T> locally (first word = pointer).
- CV_BordelloScript rows share one TU: CreateCustomers' landed model (shared CScriptThing/CTimer/CScriptBase headers plus inline
  ~CScriptThing and CScriptThing::operator= after the include) matched AdjustTavernPrices on the first probe.
- The engine/CTimer.h setter is operator=(long) (Ego_r ??4CTimer@@QAEAAV0@J@Z); do not invent SetTime.
