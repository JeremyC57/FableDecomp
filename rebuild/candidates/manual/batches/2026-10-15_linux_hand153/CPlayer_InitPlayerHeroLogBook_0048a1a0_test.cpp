// Behaviour test for 0x0048a1a0: InitPlayerHeroLogBook: a new empty log book with an empty map header.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
#include <cstdlib>
struct Node { bool color; Node* parent; Node* left; Node* right; long v[4]; };
struct Book { void* q0; void* q1; void* q2; long viewing; Node* header; unsigned int count; char cmp;
  Book() : q0(0), q1(0), q2(0), viewing(0) { header = (Node*)std::malloc(sizeof(Node)); count = 0; header->color = false; header->parent = 0; header->left = header; header->right = header; } };
struct Holder { Book* b; int resets; void Reset(Book* n) { ++resets; b = n; } };
static int run() { Holder h = {0, 0}; h.Reset(new Book); Book* b = h.b;
  CHECK(h.resets == 1 && b && !b->q0 && !b->q2 && b->viewing == 0 && b->count == 0 && !b->header->color && !b->header->parent && b->header->left == b->header && b->header->right == b->header);
  std::free(b->header); delete b; return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("0048a1a0_TEST FAIL\n"); return 1; }
    std::printf("0048a1a0_TEST PASS\n");
    return 0;
}
