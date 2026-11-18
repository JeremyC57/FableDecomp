#ifndef FABLE_ENGINE_MICROTHREADSTACKSWITCH_H
#define FABLE_ENGINE_MICROTHREADSTACKSWITCH_H
// PROPOSAL ONLY (e14, row 0x009D8650; not in hdr/: needs an owner decision, see NOTES.md).
// Retail-asm-helper: MicroThread::MicroThreadSleep
// Retail-asm: 0x009D865D `mov esp, dword ptr [0x013CA898]` and 0x009D8663 `mov dword ptr [0x013CA898], eax` (eax = the old esp):
//   the function swaps the stack pointer with MicroThread::SavedStackPointer, saving / restoring ebx ebp esi edi and the SEH head
//   fs:[0] on each stack. No C++ spelling or intrinsic writes esp; Ego_d.pdb's /Od length (36) equals retail's, so the original
//   body was a naked asm function too.
namespace MicroThread { extern unsigned char* SavedStackPointer; }   // 0x013CA898
#ifndef FABLE_PORTABLE
#define FABLE_MICROTHREAD_SLEEP_DEFINITION \
    __declspec(naked) void __fastcall MicroThreadSleep() { \
        __asm push ebx __asm push ebp __asm push esi __asm push edi \
        __asm mov eax, dword ptr fs:[0] __asm push eax \
        __asm mov eax, esp __asm mov esp, SavedStackPointer __asm mov SavedStackPointer, eax \
        __asm pop eax __asm mov dword ptr fs:[0], eax \
        __asm pop edi __asm pop esi __asm pop ebp __asm pop ebx __asm ret }
#endif
#endif
