#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
// counted UI definition, as the candidate's CUIDef / CUIDefPointer (release deletes at zero)
struct CUIDef {
 long RefCount; bool PCStyle; unsigned long StateOnRightClicked; int Deleted;
 void DeleteSelf() { ++Deleted; }
 void Release() { --RefCount; if (RefCount == 0) DeleteSelf(); }
};
struct CUIDefPointer {
 CUIDef* Object;
 explicit CUIDefPointer(CUIDef* p) : Object(p) { if (p) ++p->RefCount; }
 CUIDefPointer(const CUIDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
 ~CUIDefPointer() { if (Object) { Object->Release(); Object = 0; } }
 CUIDef* operator->() const { return Object; }
};
struct CButton {
 CUIDef* Def; float CurrentTime; float LastEventTime; float EventDelay; bool WasHovered;
 CButton* Parent; unsigned long Type; bool Deletions;
 long PActionsOnLeftUnclicked; long PActionsOnRightClicked;
 CUIDefPointer LoadAndReturnDef() const { return CUIDefPointer(Def); }
 void ClickableInitialise() { Log("CI"); }
 void ChangingInitialise() { Log("SI"); }
 void ClickableProcessEvent(long e) { LogN("PE", e); }
 void ChangeState(unsigned long s) { LogN("CS", (long)s); }
 void ObserveEvent(long e) { LogN("O", e); }
 void IgnoreEvent(long e) { LogN("I", e); }
 void OnUnhoveredSlot() { Log("UH"); }   // virtual OnUnhovered (slot 144)
 void GoForward() { Log("GF"); }
 void GoBackwards() { Log("GB"); }
 void ExecuteActions(long list) { LogN("EA", list); }
 unsigned long GetType() const { Log("GT"); return Type; }
 bool AreThereAnyDeletions() { Log("AD"); return Deletions; }
 void ScrollUntilChild(CButton* c) { Log(c ? "SU" : "SU0"); }
 void BaseAddChild(long c) { LogN("AC", c); }
 void BaseAddChildDynamically(long c) { LogN("ACD", c); }
 void ProcessEvent(long event)
 {
  if (CurrentTime - LastEventTime < EventDelay)
   return;
  if (LoadAndReturnDef()->PCStyle)
   ClickableProcessEvent(event);
 }
};
int main()
{
 CUIDef d; d.RefCount = 1; d.PCStyle = true; d.Deleted = 0;
 CButton b; b.Def = &d; b.CurrentTime = 1.0f; b.LastEventTime = 0.9f; b.EventDelay = 0.5f;
 Reset(); b.ProcessEvent(0x1a);
 if (!Is("")) { printf("0054dbc0_TEST FAIL\n"); return 1; }
 b.CurrentTime = 2.0f; Reset(); b.ProcessEvent(0x1a);
 if (!Is("PE1a ") || d.RefCount != 1) { printf("0054dbc0_TEST FAIL\n"); return 1; }
 d.PCStyle = false; Reset(); b.ProcessEvent(0x1b);
 if (!Is("") || d.RefCount != 1) { printf("0054dbc0_TEST FAIL\n"); return 1; }
 printf("0054dbc0_TEST PASS\n");
 return 0;
}