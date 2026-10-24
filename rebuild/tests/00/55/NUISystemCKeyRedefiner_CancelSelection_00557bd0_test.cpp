#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* tag, long v) { char buf[32]; std::sprintf(buf, "%s(%ld)", tag, v); std::strcat(g_seq, buf); }
namespace NUISystem {
struct CObserver;
struct CClickableBase { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void SetMode(long m) { note("3", m); } virtual void SetState(long s) { note("4", s); } };
static void* g_removed = 0;
struct CObservable { virtual void v00() { } virtual void v01() { } virtual void AddObserver(CObserver*) { std::strcat(g_seq, "A"); } virtual void AddExclusiveObserver(CObserver*) { std::strcat(g_seq, "X"); } virtual void RemoveExclusiveObserver(CObserver* o) { std::strcat(g_seq, "R"); g_removed = o; } };
struct CManager : CObservable { static CManager* __fastcall GetInstance(); };
static CManager g_manager; CManager* __fastcall CManager::GetInstance() { return &g_manager; }
struct CRoot { virtual void v00() { } };
struct CKeyRedefiner;
static CKeyRedefiner* g_pSelected = 0;
struct CKeyRedefiner : CRoot, CClickableBase { void UpdateKeyText() { std::strcat(g_seq, "T"); }
    void CancelSelection() { if (g_pSelected == this) { g_pSelected = 0; UpdateKeyText(); SetState(0x21); SetMode(0x19); CManager::GetInstance()->RemoveExclusiveObserver((CObserver*)(CClickableBase*)this); } } };
}
int main()
{
    using namespace NUISystem;
    CKeyRedefiner r; CClickableBase* control = &r; g_pSelected = &r; r.CancelSelection();
    if (std::strcmp(g_seq, "T4(33)3(25)R") || g_pSelected != 0 || g_removed != (void*)control) { std::printf("00557bd0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; CKeyRedefiner other; g_pSelected = &other; r.CancelSelection(); if (std::strcmp(g_seq, "") || g_pSelected != &other) { std::printf("00557bd0_TEST FAIL\n"); return 1; }
    std::printf("00557bd0_TEST PASS\n");
    return 0;
}
