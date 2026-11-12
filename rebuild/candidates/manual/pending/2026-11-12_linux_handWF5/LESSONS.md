## 0x009ADAE0

- A constant bitfield clear that retail spells `mov al,[m] ; and al,imm ; mov [m],al` (not `and byte ptr [m],imm`) is an inline bool setter called with a constant (CDefClassBase::SetAsTemplate(false), row 0x009ADAE0); a bool local does not reproduce it.

## 0x00499AA0

A callee the old local model declared without a body (CCountedPointer operator=) can cost dead stores in the caller: with the shared header body VC7.1 calls the COMDAT by itself and keeps the temporary dtor stores (0x00499AA0).

## 0x00EF4320

implicit CScriptThing copy + /d2inlT23..28 gives the 'two argument copies expanded, later ones called' mix in entity Mains (0x00EF4320); try the T sweep before a tagged InlineCopy ctor

## 0x00A11D10

VC7.1 re-sorts three-term x87 sums of stack-held temporaries by context, not source order: when a 3x4 inverse's determinant / translation sums come out 3,1,2 instead of 1,3,2, change how the result is consumed (named C3DVector + two-argument CMatrix3x4::TransformPoint, not operator*) -- and count the FableWin debug copy's instructions against the candidate statement lists to pick the form (0x00A11D10).

## 0x007F53B0

A copy-then-end() order around a tree _M_find (key copy-constructed first, header word read after) is an inline helper with a BY-VALUE key parameter, not an explicit temporary at the call site (0x007F53B0).

## 0x009692C0

new lesson: an early-exit block that VC7.1 sinks to the end under one '||' condition stays in place when the source has two separate ifs with identical bodies (0x009692C0)

## 0x0094D780

A def-handle function with no `sub esp` (handles homed in dead argument slots) needs no stand-ins: the shared real template bodies FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY give it (0x0094D780, first grade).

## 0x0064C869

IS_THING_ALIVE residue of the hero frame slot (own slot + reload after the call) = callee body not visible: use ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE + _NOINLINE (0x0064C869).

## 0x0095E0B0

inline-budget switches (IS_THING_ALIVE / IP copy / action dtors going out of line mid-function) come by themselves once the real inline bodies are visible; an esi/edi swap of this vs &member was the exit structure (per-step Terminate();return; vs nested ifs)

