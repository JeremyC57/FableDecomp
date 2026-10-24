#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {} };
struct CActionPointer { void* Data; void* Info; int Shares;
 void ShareData(void* d, void* i) { Data = d; Info = i; ++Shares; }
 void operator=(const CActionPointer& p) { ShareData(p.Data, p.Info); } };
struct CChild { unsigned long State; bool Def; bool Trans; bool HasStateDefinition(unsigned long s) { LogN("D", (long)s); return Def; } bool HasStateTransition(unsigned long s) { LogN("T", (long)s); return Trans; } void ChangeState(unsigned long s) { State = s; } };
template<class T> struct CRetailVector { T* First; T* Last; T* End; unsigned long size() const { return (unsigned long)(Last - First); } T& operator[](unsigned long i) { return First[i]; } };
struct CList {
 long SelectedChild; CRetailVector<CChild*> ScrollingChildren; CRetailVector<CChild*> NonScrollingChildren;
 C2DVector PositionOffset; C2DVector NextScrollingChildPosition; CActionPointer ActionOnScrollUp; CActionPointer ActionOnScrollDown; long DefaultChild;
 CList() : PositionOffset(0, 0), NextScrollingChildPosition(0, 0) {}
 void ScrollUp(unsigned long a, unsigned long b, bool c, bool d, bool e) { char s[64]; sprintf(s, "SU%lu,%lu,%d,%d,%d", a, b, c, d, e); Log(s); }
 void ScrollDown(unsigned long a, unsigned long b, bool c, bool d, bool e) { char s[64]; sprintf(s, "SD%lu,%lu,%d,%d,%d", a, b, c, d, e); Log(s); }
 void SetActionOnScrollDown(CActionPointer& a) { ActionOnScrollDown = a; }
};
int main()
{
 CChild c0, c1, c2; CChild* kids[3] = { &c0, &c1, &c2 };
 CList l; l.ScrollingChildren.First = kids; l.ScrollingChildren.Last = kids + 3; l.ScrollingChildren.End = kids + 3;
 int x, y; CActionPointer a; a.Data = &x; a.Info = &y; a.Shares = 0; l.ActionOnScrollDown.Shares = 0;
 l.SetActionOnScrollDown(a);
 if (l.ActionOnScrollDown.Data != &x || l.ActionOnScrollDown.Info != &y || l.ActionOnScrollDown.Shares != 1) { printf("0053ced4_TEST FAIL\n"); return 1; }
 printf("0053ced4_TEST PASS\n");
 return 0;
}