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
 void AddChildDynamically(CButton* child, long ref)
 {
  child->GetType();
  BaseAddChildDynamically(ref);
 }
};
int main()
{
 CButton child; child.Type = 2;
 CButton b;
 Reset(); b.AddChildDynamically(&child, 0x43);
 if (!Is("GT ACD43 ")) { printf("0053d170_TEST FAIL\n"); return 1; }
 printf("0053d170_TEST PASS\n");
 return 0;
}