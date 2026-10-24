#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
struct CComp {
 unsigned long CurrentState; unsigned long TargetState; bool Changed; long Selected; long MinToDraw; long MaxToDraw; bool ForceStateChange; CComp* Parent;
 void BaseUpdate(float t) { LogN("U", (long)t); }
 bool ChangedStateLastUpdate() { return Changed; }
 void ChangeState(unsigned long s) { LogN("CS", (long)s); }
 long GetSelectedChild() { Log("GS"); return Selected; }
 void Update(float time)
 {
  BaseUpdate(time);
  CComp* list = Parent;
  if (list->GetSelectedChild() <= MaxToDraw && list->GetSelectedChild() >= MinToDraw) {
   if (TargetState != 1) { ForceStateChange = true; ChangeState(1); }
  } else {
   if (TargetState != 0) { ForceStateChange = true; ChangeState(0); }
  }
 }
};
int main()
{
 CComp list; list.Selected = 3;
 CComp a; a.Parent = &list; a.MinToDraw = 1; a.MaxToDraw = 4; a.TargetState = 0; a.ForceStateChange = false;
 Reset(); a.Update(1.0f);
 if (!Is("U1 GS GS CS1 ") || !a.ForceStateChange) { printf("0054c2a0_TEST FAIL\n"); return 1; }
 a.TargetState = 1; a.ForceStateChange = false; Reset(); a.Update(1.0f);
 if (!Is("U1 GS GS ") || a.ForceStateChange) { printf("0054c2a0_TEST FAIL\n"); return 1; }
 list.Selected = 9; Reset(); a.Update(1.0f);
 if (!Is("U1 GS CS0 ") || !a.ForceStateChange) { printf("0054c2a0_TEST FAIL\n"); return 1; }
 list.Selected = 0; a.TargetState = 0; a.ForceStateChange = false; Reset(); a.Update(1.0f);
 if (!Is("U1 GS GS ") || a.ForceStateChange) { printf("0054c2a0_TEST FAIL\n"); return 1; }
 printf("0054c2a0_TEST PASS\n");
 return 0;
}