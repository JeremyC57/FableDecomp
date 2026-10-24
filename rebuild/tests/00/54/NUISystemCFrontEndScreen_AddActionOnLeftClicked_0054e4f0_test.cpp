#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
struct CCPPointerInfo { long Count; int Destroyed; };
static int g_deleted;
struct CActionPointer {
 void* Data; CCPPointerInfo* Info;
 CActionPointer& operator=(const CActionPointer& o)
 {
  void* data = o.Data; CCPPointerInfo* info = o.Info;
  if (Info != info) {
   if (Info) { if (--Info->Count == 0) { ++Info->Destroyed; ++g_deleted; } }
   Data = data; Info = info;
   if (info) ++info->Count;
  }
  return *this;
 }
};
static unsigned short g_key; static bool g_ime;
struct CFrontEndScreen {
 float Time; float m_fLastInputTime; CActionPointer PLeftClickAction;
 void GotoPreviousScreen(bool b) { Log(b ? "GP1" : "GP0"); }
 void Action(CActionPointer&) { Log("ACT"); }
 void AddActionOnLeftClicked(CActionPointer& a) { PLeftClickAction = a; }
};
int main()
{
 int x; CCPPointerInfo i1; i1.Count = 1; i1.Destroyed = 0; CCPPointerInfo i2; i2.Count = 1; i2.Destroyed = 0;
 CFrontEndScreen s; s.PLeftClickAction.Data = 0; s.PLeftClickAction.Info = &i1; i1.Count = 2;
 CActionPointer a; a.Data = &x; a.Info = &i2; g_deleted = 0;
 s.AddActionOnLeftClicked(a);
 if (s.PLeftClickAction.Data != &x || s.PLeftClickAction.Info != &i2 || i2.Count != 2 || i1.Count != 1 || g_deleted) { printf("0054e4f0_TEST FAIL\n"); return 1; }
 CActionPointer b; b.Data = 0; b.Info = &i1;
 s.AddActionOnLeftClicked(b);
 if (s.PLeftClickAction.Info != &i1 || i2.Count != 1 || i1.Count != 2) { printf("0054e4f0_TEST FAIL\n"); return 1; }
 s.AddActionOnLeftClicked(b);
 if (i1.Count != 2) { printf("0054e4f0_TEST FAIL\n"); return 1; }
 i2.Count = 1; s.PLeftClickAction.Info = &i2; i1.Count = 1;
 CActionPointer z; z.Data = 0; z.Info = 0; s.AddActionOnLeftClicked(z);
 if (s.PLeftClickAction.Info != 0 || i2.Destroyed != 1 || g_deleted != 1) { printf("0054e4f0_TEST FAIL\n"); return 1; }
 printf("0054e4f0_TEST PASS\n");
 return 0;
}