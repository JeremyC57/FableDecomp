#include <cstdio>
#include <cstdlib>
// Behaviour of CParticleSystem::DeleteParticle (0x00AF5330): the particle is cleared and deleted, its node unlinked
// and freed, NoActiveParticles drops by one and the iterator to the following node is returned.
static int g_deleted = 0; static int g_cleared = 0;
struct CParticle { virtual ~CParticle() { ++g_deleted; } };
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
    typedef CRetailList<CParticle*>::iterator ParticleIterator;
    CRetailList<CParticle*> Particles;
    long NoActiveParticles;
    void ClearParticle(ParticleIterator&) { ++g_cleared; }
    ParticleIterator DeleteParticle(ParticleIterator& it)
    {
        ClearParticle(it);
        delete *it;
        ParticleIterator next = Particles.erase(it);
        --NoActiveParticles;
        return next;
    }
};
int main()
{
    SListNode<CParticle*> head;
    SListNode<CParticle*>* a = (SListNode<CParticle*>*)std::malloc(sizeof(SListNode<CParticle*>));
    SListNode<CParticle*>* b = (SListNode<CParticle*>*)std::malloc(sizeof(SListNode<CParticle*>));
    head._M_next = a; a->_M_next = b; b->_M_next = &head; head._M_prev = b; b->_M_prev = a; a->_M_prev = &head;
    a->_M_data = new CParticle; b->_M_data = 0;
    CParticleSystem s; s.Particles._M_node = &head; s.NoActiveParticles = 2;
    CParticleSystem::ParticleIterator it(a);
    CParticleSystem::ParticleIterator next = s.DeleteParticle(it);
    if (next._M_node != b || head._M_next != b || b->_M_prev != &head || g_deleted != 1 || g_cleared != 1 || s.NoActiveParticles != 1) { std::printf("00af5330_TEST FAIL\n"); return 1; }
    CParticleSystem::ParticleIterator it2(b);
    next = s.DeleteParticle(it2);
    if (next._M_node != &head || head._M_next != &head || head._M_prev != &head || g_deleted != 1 || g_cleared != 2 || s.NoActiveParticles != 0) { std::printf("00af5330_TEST FAIL\n"); return 1; }
    std::printf("00af5330_TEST PASS\n");
    return 0;
}