## 0x0079CC80

VC7.1 hoists an identical call written at the head of both arms of an if/else above the branch (call before the test, its result re-tested in each arm): a 'zero-register' / extra-callee-saved residue can be this (row 0x0079CC80).

## 0x009810B0

- (wf11, 0x009810B0) A by-value CDefPointer param released as load/dec/store where retail has in-place `dec [ecx+4] ; jne`: its address escapes to an out-of-line callee (here a temp's ctor taking it by const&); put that callee's REAL body in the TU as `__declspec(noinline)` so c2 sees it does not capture the address (probe: address-taken param -> load/dec/store always; `#pragma auto_inline(off)` instead also outlined the implicit dtor of the temp).

## 0x00A44F40

- VC7.1 alias analysis is scope-based: a pointer/reference bound BEFORE a block-local object's scope opens is assumed not to alias it (loads hoisted above its stores, ctor's dead stores removed); retail keeping a dead ctor store + strict store-then-load order => the source reference was (re)bound inside that scope (0x00A44F40).
- A named local copy of a re-read field (const unsigned long crc = source.CRC) can flip esi/edi allocation and cmp operand direction even though retail reloads it from memory (0x00A44F40).

## 0x00D38D20

- TRetVoid _Fail_Val (script-macro return form) changes VC7.1 exit-tail/return-copy layout; try it whenever a script row's only residue is cleanup-tail order.

## 0x00E36750

- TRetVoid _Fail_Val + return _Fail_Val.Return() changes VC7.1 exit-block layout (in-place vs end-of-function); try it whenever a script row's exits differ only by placement.

