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
  if (ChangedStateLastUpdate()) {
   if (CurrentState == 6)
    ChangeState(0);
   if (CurrentState == 5)
    ChangeState(3);
  }
 }
};
int main()
{
 CComp c; c.Changed = true; c.CurrentState = 6;
 Reset(); c.Update(2.0f);
 if (!Is("U2 CS0 ")) { printf("0054c430_TEST FAIL\n"); return 1; }
 c.CurrentState = 5; Reset(); c.Update(1.0f);
 if (!Is("U1 CS3 ")) { printf("0054c430_TEST FAIL\n"); return 1; }
 c.Changed = false; Reset(); c.Update(1.0f);
 if (!Is("U1 ")) { printf("0054c430_TEST FAIL\n"); return 1; }
 printf("0054c430_TEST PASS\n");
 return 0;
}