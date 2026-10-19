#include <cstdio>
struct Node { void* v; unsigned char pad[0x0c]; Node* child[4]; };
static void load(Node& n, Node* a, Node* b, Node* c, Node* d) {
    n.child[0]=a; n.child[1]=b; n.child[2]=c; n.child[3]=d;
}
int main() { Node n; Node a,b,c,d; load(n,&a,&b,&c,&d);
    if (n.child[0]!=&a || n.child[1]!=&b || n.child[2]!=&c || n.child[3]!=&d) { std::printf("00a76580_TEST FAIL\n"); return 1; }
    std::printf("00a76580_TEST PASS\n"); return 0; }
