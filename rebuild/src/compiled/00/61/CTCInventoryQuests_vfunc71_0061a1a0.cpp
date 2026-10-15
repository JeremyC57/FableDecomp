// CTCInventoryQuests::vfunc_71() -- steps the screen's static test-quest index forward, wrapping to 0 past the last entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A1A0, 33 bytes (+15 int3). rtti-vtables: CTCInventoryQuests 0x012585B4 slot 71.
// Globals: the test-quest index @ 0x013B8C64 and count @ 0x013B8C68 (module statics, referenced only by the
//          CTCInventoryQuests functions 0x00619FA2..0x0061A748).
//
// Shape: `mov eax,[idx] ; mov ecx,[count] ; inc eax ; dec ecx ; cmp eax,ecx ; mov [idx],eax ; jle ; mov [idx],0` --
// a pre-increment compared against `count - 1` (signed), the store scheduled before the branch. `this` is unused.
// The real method name is unknown (catalog label kept).
extern long g_testQuestIndex;                 // 0x013B8C64
extern long g_testQuestCount;                 // 0x013B8C68
struct CTCInventoryQuests {
    void vfunc_71();
};
void CTCInventoryQuests::vfunc_71()
{
    if (++g_testQuestIndex > g_testQuestCount - 1)
        g_testQuestIndex = 0;
}