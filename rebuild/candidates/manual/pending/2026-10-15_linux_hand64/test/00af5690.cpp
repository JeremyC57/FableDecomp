#include <cstdio>
#include <cstdlib>
// Behaviour of CParticleSystem::DeleteDecal (0x00AF5690): the decal is cleared, destroyed and returned to its pool
// (class operator delete), its node unlinked and freed, NoActiveDecals drops by one, the next iterator returned.
static int g_destroyed = 0; static int g_pooled = 0; static int g_cleared = 0;
struct CDecal {
    int m_payload;
    ~CDecal() { ++g_destroyed; }
    static void operator delete(void* p) { ++g_pooled; std::free(p); }
    static void* operator new(size_t n) { return std::malloc(n); }
};
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    struct iterator {
        SListNode<T>* _M_node;
        iterator() {}
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
    };
    SListNode<T>* _M_node;
    iterator erase(iterator pos)
    {
        SListNode<T>* next = pos._M_node->_M_next;
        SListNode<T>* prev = pos._M_node->_M_prev;
        SListNode<T>* n = pos._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        std::free(n);
        return iterator(next);
    }
};
struct CParticleSystem {
    typedef CRetailList<CDecal*>::iterator DecalIterator;
    CRetailList<CDecal*> Decals;
    long NoActiveDecals;
    void ClearDecal(DecalIterator&) { ++g_cleared; }
    DecalIterator DeleteDecal(DecalIterator& it)
    {
        ClearDecal(it);
        delete *it;
        DecalIterator next = Decals.erase(it);
        --NoActiveDecals;
        return next;
    }
};
int main()
{
    SListNode<CDecal*> head;
    SListNode<CDecal*>* a = (SListNode<CDecal*>*)std::malloc(sizeof(SListNode<CDecal*>));
    head._M_next = a; a->_M_next = &head; head._M_prev = a; a->_M_prev = &head;
    a->_M_data = new CDecal;
    CParticleSystem s; s.Decals._M_node = &head; s.NoActiveDecals = 5;
    CParticleSystem::DecalIterator it(a);
    CParticleSystem::DecalIterator next = s.DeleteDecal(it);
    if (next._M_node != &head || head._M_next != &head || head._M_prev != &head || g_destroyed != 1 || g_pooled != 1 || g_cleared != 1 || s.NoActiveDecals != 4) { std::printf("00af5690_TEST FAIL\n"); return 1; }
    std::printf("00af5690_TEST PASS\n");
    return 0;
}
