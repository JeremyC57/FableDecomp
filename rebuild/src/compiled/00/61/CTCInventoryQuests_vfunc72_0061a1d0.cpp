// CTCInventoryQuests::vfunc_72() -- steps the screen's static test-quest index back, wrapping to the last entry below 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A1D0, 20 bytes (+12 int3). rtti-vtables: CTCInventoryQuests 0x012585B4 slot 72.
// Globals: the test-quest index @ 0x013B8C64 and count @ 0x013B8C68 (module statics).
//
// Shape: `dec dword ptr [idx] ; jns ; mov eax,[count] ; dec eax ; mov [idx],eax` -- `if (--idx < 0)` folds into a
// memory decrement and a sign test. `this` is unused. The real method name is unknown (catalog label kept).
extern long g_testQuestIndex;                 // 0x013B8C64
extern long g_testQuestCount;                 // 0x013B8C68
class CTCInventoryQuests { public:
    void vfunc_72();
};
void CTCInventoryQuests::vfunc_72()
{
    if (--g_testQuestIndex < 0)
        g_testQuestIndex = g_testQuestCount - 1;
}