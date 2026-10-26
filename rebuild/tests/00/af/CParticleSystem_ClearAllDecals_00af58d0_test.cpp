#include <cstdio>
// Behaviour of CParticleSystem::ClearAllDecals (0x00AF58D0): ClearDecal sees every node of the decals list in order,
// through an iterator that points at the current node; an empty list makes no call.
struct CParticle; struct CDecal; class CEnginePrimitiveHandle;
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    struct iterator {
        SListNode<T>* _M_node;
        iterator() {}
        iterator(SListNode<T>* n) : _M_node(n) {}
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
};

static void* g_seen[8]; static int g_count = 0;

struct CParticleSystem {
    unsigned char m_pad00[0x1c];
    CRetailList<CParticle*> Particles;
    CRetailList<CDecal*>    Decals;
    void ClearDecal(const CRetailList<CDecal*>::iterator& it) { g_seen[g_count++] = it._M_node; }
    void ClearAllDecals(CEnginePrimitiveHandle* handle)
    {
        CRetailList<CDecal*>::iterator it = Decals.begin();
        SListNode<CDecal*>* node = it._M_node;
        while (node != Decals._M_node) {
            ClearDecal(it);
            node = node->_M_next;
            it._M_node = node;
        }
    }
};

int main()
{
    SListNode<CDecal*> head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head;
    head._M_prev = &n2; n2._M_prev = &n1; n1._M_prev = &head;
    CParticleSystem s; s.Decals._M_node = &head;
    s.ClearAllDecals(0);
    if (g_count != 2 || g_seen[0] != &n1 || g_seen[1] != &n2) { std::printf("00af58d0_TEST FAIL\n"); return 1; }
    SListNode<CDecal*> empty; empty._M_next = &empty; empty._M_prev = &empty; s.Decals._M_node = &empty;
    s.ClearAllDecals(0);
    if (g_count != 2) { std::printf("00af58d0_TEST FAIL\n"); return 1; }
    std::printf("00af58d0_TEST PASS\n");
    return 0;
}