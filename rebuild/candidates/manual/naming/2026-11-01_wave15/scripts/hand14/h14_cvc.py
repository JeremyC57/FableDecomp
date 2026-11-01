# family cvc (hand): 0x0070C770's local model put an invented `virtual void Validate()` at slot 1 of C3DAnimationStateBase; the
# RTTI vtable 0x012354D0 and engine/CIVCountedPointeeBase.h put CIVCountedPointeeBase::Delete() const at slot 1
# (DECISIONS_W13_RESOLVED 0x0070C770 row). The row now takes the shared header's retail class (FABLE_RETAIL_DEF_CLASS_BASE: vptr +
# IVRefCount, Delete at slot 1); C3DAnimationStateBase keeps GetType at slot 2. No call names either slot: name-only. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '0070c770'
edit(A, 'src', 'class CIVCountedPointeeBase {\npublic:\n    virtual ~CIVCountedPointeeBase();                   // slot 0\n    long RefCount;                                      // +0x04\n};\n',
     '#include "rebuild_abi.h"\n#define FABLE_RETAIL_DEF_CLASS_BASE   // the retail polymorphic class: slot 0 scalar deleting dtor, slot 1 Delete (vtable 0x012354D0)\n'
     '#include "engine/CIVCountedPointeeBase.h"\n')
edit(A, 'src', '    virtual void Validate();                            // slot 1\n', '')
edit(A, 'src', '//   CIVCountedPointeeBase (vptr, RefCount) + BaseAnimation', '//   CIVCountedPointeeBase (engine header retail class: vptr, IVRefCount; slot 1 Delete) + BaseAnimation')
