// Behaviour test for CEnginePrimitiveMorphedAnimatingMesh dtor (0x00709B10): the inline STLport set dtor (erase only when
// non-empty, header relinked, then freed), then the vectors, the def release and the interpolators in reverse order.
#include <cstdio>
#include <cstdlib>
static int g_order[16]; static int g_n; static int g_erased;
struct Node { long color; Node* parent; Node* left; Node* right; };
struct Set {
    Node* header; unsigned int count;
    void Erase(Node*) { ++g_erased; }
    void clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~Set() { clear(); if (header) std::free(header); g_order[g_n++] = 3; }
};
struct Tr { int id; ~Tr() { g_order[g_n++] = id; } };
struct Def { unsigned int RefCount; int destroyed; void ReleaseTemporary() { if (--RefCount == 0) ++destroyed; } };
struct DefPtr { Def* D; ~DefPtr() { if (D) { D->ReleaseTemporary(); D = 0; } g_order[g_n++] = 9; } };
struct Mesh { Tr Position, Orientation, Animation; DefPtr PDef; };
struct Morphed : Mesh { Tr Morphs, Channels; Set Bones; };
int main()
{
    Def d; d.RefCount = 1; d.destroyed = 0;
    {
        Morphed m; m.Position.id = 13; m.Orientation.id = 12; m.Animation.id = 11; m.Morphs.id = 5; m.Channels.id = 4; m.PDef.D = &d;
        m.Bones.header = (Node*)std::malloc(sizeof(Node)); m.Bones.count = 2;
    }
    const int want[7] = { 3, 4, 5, 9, 11, 12, 13 };
    for (int i = 0; i < 7; ++i) if (g_order[i] != want[i]) { std::printf("00709b10_TEST FAIL\n"); return 1; }
    if (g_erased != 1 || d.destroyed != 1) { std::printf("00709b10_TEST FAIL\n"); return 1; }
    std::printf("00709b10_TEST PASS\n");
    return 0;
}