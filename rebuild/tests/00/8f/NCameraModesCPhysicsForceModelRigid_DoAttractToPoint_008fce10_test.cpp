#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct SAttractor { C3DVector first; float second; };
struct SAttractorNode { SAttractorNode* _M_next; SAttractorNode* _M_prev; SAttractor _M_data; };
struct CAttractorList { SAttractorNode* _M_node; SAttractor& front() { return _M_node->_M_next->_M_data; } };
struct CPhysicsState { C3DVector Position; CAttractorList Attractors; };
struct CPhysicsForceModelRigid {
    CPhysicsState* LinkedState;
    C3DVector DoAttractToPoint() { return LinkedState->Attractors.front().first; }
};
int main()
{
    SAttractorNode head, n1;
    head._M_next = &n1; head._M_prev = &n1; n1._M_next = &head; n1._M_prev = &head;
    n1._M_data.first.X = 1.0f; n1._M_data.first.Y = 2.0f; n1._M_data.first.Z = 3.0f; n1._M_data.second = 0.5f;
    CPhysicsState st; st.Attractors._M_node = &head;
    CPhysicsForceModelRigid f; f.LinkedState = &st;
    C3DVector r = f.DoAttractToPoint();
    if (r.X != 1.0f || r.Y != 2.0f || r.Z != 3.0f) { std::printf("008fce10_TEST FAIL\n"); return 1; }
    std::printf("008fce10_TEST PASS\n");
    return 0;
}