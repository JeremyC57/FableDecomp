// EH unwind funclet @ 0x01211A25 of PKCDeferredBank::PKCDeferredBank (parent @ 0x00C14E40): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x2e of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136ACE0 lists 0x01211A25 in its unwind map; its __ehhandler thunk
// 0x01211A62 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C14E40.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01211A25, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x2e) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/PKCDeferredBank_PKCDeferredBank_00c14e40.cpp"