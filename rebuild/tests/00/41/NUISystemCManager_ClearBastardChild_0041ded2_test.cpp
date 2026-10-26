#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
// tree nodes chained through an in-order successor, as the STLport _Rb_tree increment does
struct CComponentPointer { int Releases; void ReleaseData() { ++Releases; } };
struct CNode { CNode* Next; CComponentPointer Value; };
static CNode* Rb_tree_increment(CNode* n) { return n->Next; }
struct CIter { CNode* Node; CIter(CNode* n) : Node(n) {} bool operator!=(const CIter& o) const { return Node != o.Node; } void operator++() { Node = Rb_tree_increment(Node); } CNode* operator->() const { return Node; } };
class CMap { public: CNode* Header; CNode* Leftmost; int Cleared; CIter begin() const { return CIter(Leftmost); } CIter end() const { return CIter(Header); } void clear() { ++Cleared; Leftmost = Header; } };
struct CManager {
 CComponentPointer BastardChild; CMap BastardChildren;
 void ClearBastardChild()
 {
  for (CIter it = BastardChildren.begin(); it != BastardChildren.end(); ++it) {
   it->Value.ReleaseData();
   BastardChild.ReleaseData();
  }
  BastardChildren.clear();
 }
};
int main()
{
 CNode header, a, b; a.Next = &b; b.Next = &header; a.Value.Releases = 0; b.Value.Releases = 0;
 CManager m; m.BastardChild.Releases = 0; m.BastardChildren.Header = &header; m.BastardChildren.Leftmost = &a; m.BastardChildren.Cleared = 0;
 m.ClearBastardChild();
 if (a.Value.Releases != 1 || b.Value.Releases != 1 || m.BastardChild.Releases != 2 || m.BastardChildren.Cleared != 1) { printf("0041ded2_TEST FAIL\n"); return 1; }
 m.ClearBastardChild();
 if (m.BastardChild.Releases != 2 || m.BastardChildren.Cleared != 2) { printf("0041ded2_TEST FAIL\n"); return 1; }
 printf("0041ded2_TEST PASS\n");
 return 0;
}