#include <cstdio>
struct Node { Node* parent; Node* left; Node* right; };
static Node* erased[3]; static Node* released[3]; static int erasedCount=0; static int releasedCount=0;
static void eraseNode(Node* p) { erased[erasedCount++]=p; }
static void releaseNode(Node* p) { released[releasedCount++]=p; }
struct Tree { Node* head; unsigned long count; unsigned long compare; void clear() { if(count) { eraseNode(head->parent); head->left=head; head->parent=0; head->right=head; count=0; } if(head) releaseNode(head); } };
struct Owner { Tree a; unsigned char gap[12]; Tree b; Tree c; ~Owner() { c.clear(); b.clear(); a.clear(); } };
int main() {
    Node ha, hb, hc, ra, rb, rc;
    ha.parent=&ra; ha.left=&ra; ha.right=&ra; hb.parent=&rb; hb.left=&rb; hb.right=&rb; hc.parent=&rc; hc.left=&rc; hc.right=&rc;
    { Owner o; o.a.head=&ha; o.a.count=1; o.a.compare=0; o.b.head=&hb; o.b.count=1; o.b.compare=0; o.c.head=&hc; o.c.count=1; o.c.compare=0; }
    if(erasedCount!=3 || erased[0]!=&rc || erased[1]!=&rb || erased[2]!=&ra || releasedCount!=3 || released[0]!=&hc || released[1]!=&hb || released[2]!=&ha ||
       ha.left!=&ha || ha.parent!=0 || ha.right!=&ha || hb.left!=&hb || hb.parent!=0 || hb.right!=&hb || hc.left!=&hc || hc.parent!=0 || hc.right!=&hc) { std::printf("00ad7400_TEST FAIL\n"); return 1; }
    std::printf("00ad7400_TEST PASS\n"); return 0;
}