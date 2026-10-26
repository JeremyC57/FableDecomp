// Behaviour test for NCameraModes::CPhysicsState::CPhysicsState (0x0084F490): two self-linked sentinels, cull flag cleared.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
class C3DVector { public: float X, Y, Z; };
template <class T1, class T2> struct Pair { T1 first; T2 second; };
template <class T> class List {
public:
    struct node { node* next; node* prev; T data; };
    node* _M_node;
    List() : _M_node(0)
    {
        node* sentinel = (node*)std::malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        _M_node = sentinel;
    }
};
class CPhysicsState {
public:
    CPhysicsState() : CullPhysics(false) {}
    C3DVector Position, Velocity, Acceleration;
    List<Pair<C3DVector, float> > Attractors;
    List<Pair<C3DVector, float> > IncidentalAttractors;
    bool CullPhysics;
};
int main()
{
    static unsigned char raw[sizeof(CPhysicsState)];
    std::memset(raw, 0xCD, sizeof(raw));
    CPhysicsState* s = new (raw) CPhysicsState;
    if (s->CullPhysics || !s->Attractors._M_node || s->Attractors._M_node->next != s->Attractors._M_node || s->Attractors._M_node->prev != s->Attractors._M_node) { std::printf("0084f490_TEST FAIL\n"); return 1; }
    if (!s->IncidentalAttractors._M_node || s->IncidentalAttractors._M_node == s->Attractors._M_node || s->IncidentalAttractors._M_node->next != s->IncidentalAttractors._M_node) { std::printf("0084f490_TEST FAIL\n"); return 1; }
    std::free(s->Attractors._M_node); std::free(s->IncidentalAttractors._M_node);
    std::printf("0084f490_TEST PASS\n");
    return 0;
}