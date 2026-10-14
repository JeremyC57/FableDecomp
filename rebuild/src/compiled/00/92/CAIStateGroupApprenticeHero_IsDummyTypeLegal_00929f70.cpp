// CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(long dummyType) -- static: is the practice dummy of this type free?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929F70, length 68 bytes (+ int3 padding).
// No callees. Globals: three "dummy in use" bools @ 0x013BAE18 / 0x013BAE19 / 0x013BAE1A
// (one per dummy type 0..2). A three-case switch (`sub eax,0 ; je ; dec ; je ; dec ; je`)
// returns `!g_used` for each type (`xor eax,eax ; test cl,cl ; sete al`) and false for any
// other value (`xor al,al`). `ret 4` with no `this`: a static member compiled __stdcall (this TU is built with /Gz, the callee cleans its one argument).
extern bool g_dummyTypeInUse0;              // 0x013BAE18
extern bool g_dummyTypeInUse1;              // 0x013BAE19
extern bool g_dummyTypeInUse2;              // 0x013BAE1A

struct CAIStateGroup_ApprenticeHero {
    static bool __stdcall IsDummyTypeLegal(long dummyType);
};

bool __stdcall CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(long dummyType)
{
    switch (dummyType) {
    case 0: return !g_dummyTypeInUse0;
    case 1: return !g_dummyTypeInUse1;
    case 2: return !g_dummyTypeInUse2;
    }
    return false;
}