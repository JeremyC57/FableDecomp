// NCameraModes::CPhysicsState::CPhysicsState() -- builds the two empty attractor lists (STLport sentinel nodes) and clears the cull flag; the position / velocity / acceleration vectors are left uninitialised
// Leaf: ??0CPhysicsState@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084F490, 58 bytes, plain `ret` (returns this).
// Identity: Ego_r.pdb NCameraModes::CPhysicsState (0x40 bytes there): Position +0, Velocity +0xc, Acceleration +0x18,
//   Attractors +0x24 and IncidentalAttractors list<pair<C3DVector, float> > (16-byte element -> 0x18-byte node),
//   CullPhysics bool. With 4-byte STLport lists the retail offsets are +0x24 / +0x28 / +0x2c -- exactly the three
//   members this body writes. The manifest label "DataStructure_InitListPair" is descriptive.
// Callee: malloc 0x00BFEA0E (STLport's node allocator in this build), once per list sentinel.
// Shape: each STLport list constructor nulls its node proxy, mallocs the 0x18-byte sentinel, links it to itself and
//   only then stores it (the landed list constructor 0x0084F1F0 model); the bool is an init-list member after the lists.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class C3DVector { public: float X, Y, Z; };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> class list {                         // STLport list: one pointer to the sentinel node
    struct node { node* next; node* prev; T data; };
    node* _M_node;
public:
    list() : _M_node(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        _M_node = sentinel;
    }
};
}

namespace NCameraModes {
class CPhysicsState {
public:
    CPhysicsState();
    C3DVector Position;                                          // +0x00
    C3DVector Velocity;                                          // +0x0c
    C3DVector Acceleration;                                      // +0x18
    std::list<std::pair<C3DVector, float> > Attractors;          // +0x24
    std::list<std::pair<C3DVector, float> > IncidentalAttractors;   // +0x28
    bool CullPhysics;                                            // +0x2c
};

CPhysicsState::CPhysicsState()
    : CullPhysics(false)
{
}
}