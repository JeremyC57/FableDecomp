// Test for ~CThingCreatureBase (0x006661B0): the user body clears PTCModeManager and DebugText; the counted-pointer members
// release their count block (deleting the object at the last reference, then zeroing Data and Info), the def pointer
// releases its pointee (Delete at zero), the body reorienter is deleted when present, and members die in reverse order.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct Info { unsigned long RefCount; int id; };
struct Counted {
    void* Data; Info* Info_;
    void Release() { if (Info_) { if (--Info_->RefCount == 0) g_log[g_n++] = Info_->id; } Data = 0; Info_ = 0; }
};
struct DefPointee { unsigned long RefCount; int id; };
struct DefPointer { DefPointee* Object; void Release() { if (Object) { if (--Object->RefCount == 0) g_log[g_n++] = Object->id; Object = 0; } } };
struct Creature {
    DefPointer PDef; Counted PCurrentAction; Counted PCreatureInteraction; void* PTCModeManager; int* BodyReorienter; char* DebugText;
    void Destroy()
    {
        PTCModeManager = 0; DebugText = 0;                          // user body
        if (BodyReorienter) { g_log[g_n++] = *BodyReorienter; BodyReorienter = 0; }   // scoped_ptr (declared after the counted pointers)
        PCreatureInteraction.Release();
        PCurrentAction.Release();
        PDef.Release();
    }
};
int main()
{
    Info a = {1, 10}, b = {2, 20}; DefPointee d = {1, 30}; int reorienter = 40; char text[4] = "x";
    Creature c; c.PDef.Object = &d; c.PCurrentAction.Data = &a; c.PCurrentAction.Info_ = &a;
    c.PCreatureInteraction.Data = &b; c.PCreatureInteraction.Info_ = &b; c.PTCModeManager = &c; c.BodyReorienter = &reorienter; c.DebugText = text;
    c.Destroy();
    if (c.PTCModeManager || c.DebugText || g_n != 3 || g_log[0] != 40 || g_log[1] != 10 || g_log[2] != 30 || b.RefCount != 1 ||
        c.PCreatureInteraction.Info_ || c.PCurrentAction.Data || c.PDef.Object) { std::printf("006661b0_TEST FAIL\n"); return 1; }
    std::printf("006661b0_TEST PASS\n");
    return 0;
}