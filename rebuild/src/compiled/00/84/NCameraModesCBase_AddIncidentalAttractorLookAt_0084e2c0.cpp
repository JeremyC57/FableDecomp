// NCameraModes::CBase::AddIncidentalAttractorLookAt(const C3DVector&, float) -- adds an incidental look-at attractor (position, weight) to the mode's look-at physics, but only when the camera tools see a clear line from the camera's go-to position to it
// Leaf: ?AddIncidentalAttractorLookAt@CBase@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E2C0, 151 bytes, __fastcall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?AddIncidentalAttractorLookAt@CBase@NCameraModes@@IAEXABVC3DVector@@M@Z, the fourth of the
//   AddAttractorGoTo / AddIncidentalAttractorGoTo / AddAttractorLookAt / AddIncidentalAttractorLookAt run (TLC 0x0084E1A0 /
//   0x0084E200 / 0x0084E260 / 0x0084E2C0, lists +0x78 / +0x7c / +0x34 / +0x38); only this one tests line of sight in
//   both PDBs (the larger body). The manifest label "LinkedList_InsertIfLOS" is an autoname.
// Callees: NCameraModes::CTools::IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos,
//   C3DVector& hitNormal, float& distance, const CThing** hitThing, ELOSTest, bool) const 0x00877C40 (the Ego_r overload
//   without the thing vector; manifest "CTools::IsLineOfSightFiltered"), malloc 0x00BFEA0E (the STLport list node).
// Layout: CBase LookAt CPhysicsState +0x10 (its IncidentalAttractors list +0x28 -> +0x38), camera manager reference +0x40
//   (its CTools at +0x78), GoTo CPhysicsState +0x54 (Position first).
// Shape: the camera manager is a POINTER local taken first (`mov eax,[edi+40h]` before the pushes, `lea ecx,[eax+78h]` at
//   the call; a `const CTools&` local or the inline GetCamTools() sinks the load to the call); STLport
//   list::push_back(x) = insert(end(), x): the sentinel is read before the malloc'd 0x18-byte node, null-checked placement
//   copy (the pair's weight copy-propagated from the argument slot), then the link with `prev` read first; the out
//   distance is homed in the dead `pos` argument slot.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class C3DVector {
public:
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float X, Y, Z;
};

class CThing;

namespace std {
template <class T1, class T2> struct pair {
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    T1 first;
    T2 second;
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& x, const T2& y) { return pair<T1, T2>(x, y); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_node_base* _M_node;
};
template <class T> class list {                         // STLport list: one pointer to the sentinel node
public:
    typedef _List_iterator<T> iterator;
    iterator end() { return iterator(_M_node); }
    void insert(iterator position, const T& x)
    {
        _List_node<T>* node = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        ::new (&node->_M_data) T(x);
        _List_node_base* prev = position._M_node->_M_prev;
        node->_M_next = position._M_node;
        node->_M_prev = prev;
        prev->_M_next = node;
        position._M_node->_M_prev = node;
    }
    void push_back(const T& x) { insert(end(), x); }
    _List_node_base* _M_node;
};
}

namespace NCameraModes {
class CTools {
public:
    enum ELOSTest { };
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, float& distance,
                       const CThing** hitThing, ELOSTest test, bool flag) const;   // 0x00877C40
};

class CGameCameraManagerView {
public:
    unsigned char _pad_0x00[0x78];
    CTools        Tools;                                            // +0x78
};

class CPhysicsState {
public:
    C3DVector Position;                                             // +0x00
    C3DVector Velocity;                                             // +0x0c
    C3DVector Acceleration;                                         // +0x18
    std::list<std::pair<C3DVector, float> > Attractors;            // +0x24
    std::list<std::pair<C3DVector, float> > IncidentalAttractors;  // +0x28
    bool CullPhysics;                                               // +0x2c
};

class CBase {
public:
    const CTools& GetCamTools() const { return CameraManager->Tools; }
protected:
    void AddIncidentalAttractorLookAt(const C3DVector& pos, float weight);
public:
    unsigned char           _pad_0x00[0x10];
    CPhysicsState           LookAtPhysics;                          // +0x10
    CGameCameraManagerView* CameraManager;                          // +0x40
    unsigned char           _pad_0x44[0x10];
    CPhysicsState           GoToPhysics;                            // +0x54
};

void CBase::AddIncidentalAttractorLookAt(const C3DVector& pos, float weight)
{
    CGameCameraManagerView* manager = CameraManager;
    C3DVector hitPos;
    C3DVector hitNormal;
    float distance;
    const CThing* hitThing = 0;
    if (manager->Tools.IsLineOfSight(GoToPhysics.Position, pos, hitPos, hitNormal, distance, &hitThing, (CTools::ELOSTest)7, true))
        LookAtPhysics.IncidentalAttractors.push_back(std::make_pair(pos, weight));
}
}