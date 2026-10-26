#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
class CCPPointerInfo { public: long Count; int Destroyed; };
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
 void ProcessEvent(long event)
 {
  if (Time - m_fLastInputTime < 0.2f)
   return;
  switch (event) {
  case 0x1b:
   GotoPreviousScreen(false);
   break;
  case 0x21:
  case 0x22:
   if (g_key == 1 && !g_ime)
    GotoPreviousScreen(true);
   break;
  case 0x1a:
   if (PLeftClickAction.Data)
    Action(PLeftClickAction);
   break;
  }
  if (event != 0x19)
   m_fLastInputTime = Time;
 }
};
int main()
{
 int x; CFrontEndScreen s; s.PLeftClickAction.Data = &x; s.PLeftClickAction.Info = 0;
 s.Time = 1.0f; s.m_fLastInputTime = 0.9f; Reset(); s.ProcessEvent(0x1b);
 if (!Is("") || s.m_fLastInputTime != 0.9f) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 2.0f; Reset(); s.ProcessEvent(0x1b);
 if (!Is("GP0 ") || s.m_fLastInputTime != 2.0f) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 3.0f; Reset(); s.ProcessEvent(0x1a);
 if (!Is("ACT ")) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 4.0f; g_key = 1; g_ime = false; Reset(); s.ProcessEvent(0x22);
 if (!Is("GP1 ")) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 5.0f; g_ime = true; Reset(); s.ProcessEvent(0x21);
 if (!Is("")) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 6.0f; Reset(); s.ProcessEvent(0x19);
 if (!Is("") || s.m_fLastInputTime != 5.0f) { printf("0054e280_TEST FAIL\n"); return 1; }
 s.Time = 7.0f; Reset(); s.ProcessEvent(0x1e);
 if (!Is("") || s.m_fLastInputTime != 7.0f) { printf("0054e280_TEST FAIL\n"); return 1; }
 printf("0054e280_TEST PASS\n");
 return 0;
}