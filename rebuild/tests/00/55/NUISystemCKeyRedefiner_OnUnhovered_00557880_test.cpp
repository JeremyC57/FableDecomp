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
struct CActionInputControl { long GameAction; long ControllerType; long KeyboardKey; long XboxButton; long MouseButton; float DirX, DirY; };
struct CGameActionList {
 struct node { node* next; node* prev; long value; };
 node* head;
 void clear() { node* cur = head->next; while (cur != head) { node* tmp = cur; cur = cur->next; free(tmp); } head->next = head; head->prev = head; }
 void insert(node* pos, const long& v) { node* tmp = (node*)malloc(sizeof(node)); tmp->value = v; node* prev = pos->prev; tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp; }
 void push_back(const long& v) { insert(head, v); }
};
struct CKeyRedefiner;
static CKeyRedefiner* g_selected;
struct CKeyRedefiner {
 CUIDef* Def; unsigned long TargetState; unsigned long StateBeforeRightClick; long PActionsOnRightClicked;
 bool HasKeyText; CGameActionList Actions; long SubType; CActionInputControl Definition;
 CUIDefPointer LoadAndReturnDef() const { return CUIDefPointer(Def); }
 void ClickableOnHovered() { Log("COH"); }
 void ClickableOnLeftUnclicked() { Log("CLU"); }
 void BaseChangeState(unsigned long s) { LogN("BCS", (long)s); }
 void ChangeState(unsigned long s) { LogN("CS", (long)s); }
 void ObserveEvent(long e) { LogN("O", e); }
 void IgnoreEvent(long e) { LogN("I", e); }
 void ExecuteActions(long l) { LogN("EA", l); }
 void SetKeyTextTag(const char* tag) { Log(tag); }
 void SetKeyTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { char s[32]; sprintf(s, "C%02x%02x%02x%02x", r, g, b, a); Log(s); }
 void AddExclusiveObserver() { Log("AXO"); }
 void AddObserver(bool null) { Log(null ? "AO0" : "AO"); }
 void CancelSelection() { Log("CAN"); }
 long GetSubTypeForAction(const CActionInputControl& c) const { return c.GameAction * 2; }
 void SetActionText(const char* name) { Log(name); }
 void UpdateKeyText() { Log("UKT"); }
 void OnUnhovered() { ClickableOnHovered(); ChangeState(4); }
};
int main()
{
 CKeyRedefiner k; Reset(); k.OnUnhovered();
 if (!Is("COH CS4 ")) { printf("00557880_TEST FAIL\n"); return 1; }
 printf("00557880_TEST PASS\n");
 return 0;
}