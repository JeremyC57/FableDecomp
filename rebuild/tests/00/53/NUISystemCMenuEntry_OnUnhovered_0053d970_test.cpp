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
 void OnUnhovered();
};
struct CHoveredList {
 struct Node { Node* Next; Node* Prev; CButton* Object; };
 Node* Head;
 Node* find(CButton* p) const { Node* n = Head->Next; while (n != Head && n->Object != p) n = n->Next; return n; }
 void erase(Node* p) { if (p != Head) { Node* next = p->Next; Node* prev = p->Prev; prev->Next = next; next->Prev = prev; free(p); } }
};
static CHoveredList g_hovered;
void CButton::OnUnhovered()
{
 if (LoadAndReturnDef()->PCStyle) {
  IgnoreEvent(0x1a); IgnoreEvent(0x1f); IgnoreEvent(0x1c);
  IgnoreEvent(0x1b); IgnoreEvent(0x20); IgnoreEvent(0x1d);
 }
 g_hovered.erase(g_hovered.find(this));
}
static CHoveredList::Node* Add(CButton* p)
{
 CHoveredList::Node* n = (CHoveredList::Node*)malloc(sizeof(CHoveredList::Node));
 n->Object = p; n->Next = g_hovered.Head; n->Prev = g_hovered.Head->Prev;
 g_hovered.Head->Prev->Next = n; g_hovered.Head->Prev = n; return n;
}
static int Count() { int c = 0; for (CHoveredList::Node* n = g_hovered.Head->Next; n != g_hovered.Head; n = n->Next) ++c; return c; }
int main()
{
 CHoveredList::Node head; head.Next = &head; head.Prev = &head; head.Object = 0; g_hovered.Head = &head;
 CUIDef d; d.RefCount = 1; d.PCStyle = true; d.Deleted = 0;
 CButton a, b, c; a.Def = &d; b.Def = &d; c.Def = &d;
 Add(&a); Add(&b); Add(&c);
 Reset(); b.OnUnhovered();
 if (!Is("I1a I1f I1c I1b I20 I1d ") || Count() != 2 || head.Next->Object != &a || head.Prev->Object != &c) { printf("0053d970_TEST FAIL\n"); return 1; }
 d.PCStyle = false; Reset(); b.OnUnhovered();
 if (!Is("") || Count() != 2) { printf("0053d970_TEST FAIL\n"); return 1; }
 Reset(); a.OnUnhovered(); c.OnUnhovered();
 if (Count() != 0 || head.Next != &head || d.RefCount != 1) { printf("0053d970_TEST FAIL\n"); return 1; }
 printf("0053d970_TEST PASS\n");
 return 0;
}