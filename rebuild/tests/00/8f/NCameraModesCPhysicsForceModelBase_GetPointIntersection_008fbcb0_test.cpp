// Behaviour test for 0x008FBCB0 CPhysicsForceModelBase::GetPointIntersection: first hit in list order wins.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;
    C3DVector Normal;
    float     DistanceSquared;
};

class CPhysicsMeshObject { public:
    bool  hits;
    float hitX;
    mutable int queried;
    bool GetPointIntersectionFromTo(const C3DVector&, const C3DVector&, CMeshIntersectionData& data) const
    {
        ++queried;
        if (!hits) return false;
        data.IntersectionPosition.X = hitX; data.IntersectionPosition.Y = 2.0f; data.IntersectionPosition.Z = 3.0f;
        return true;
    }
};

struct Node { Node* next; Node* prev; CPhysicsMeshObject data; };

static bool GetPointIntersection(Node* head, int, const C3DVector& from, const C3DVector& to, C3DVector& out)
{
    CMeshIntersectionData data;
    for (Node* it = head->next; it != head; ) {
        Node* cur = it; it = it->next;
        if (cur->data.GetPointIntersectionFromTo(from, to, data)) {
            out = data.IntersectionPosition;
            return true;
        }
    }
    return false;
}

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    Node head, a, b, c;
    head.next = &a; a.next = &b; b.next = &c; c.next = &head;
    head.prev = &c; a.prev = &head; b.prev = &a; c.prev = &b;
    a.data.hits = false; b.data.hits = true; b.data.hitX = 10.0f; c.data.hits = true; c.data.hitX = 20.0f;
    a.data.queried = b.data.queried = c.data.queried = 0;
    C3DVector from = { 0, 0, 0 }, to = { 1, 1, 1 }, out = { 0, 0, 0 };
    int fail = 0;
    if (!GetPointIntersection(&head, 0, from, to, out) || !near(out.X, 10.0f) || !near(out.Z, 3.0f)) fail = 1;
    if (a.data.queried != 1 || b.data.queried != 1 || c.data.queried != 0) fail = 1;
    b.data.hits = false; c.data.hits = false; out.X = 99.0f;
    if (GetPointIntersection(&head, 0, from, to, out) || !near(out.X, 99.0f)) fail = 1;
    Node empty; empty.next = empty.prev = &empty;
    if (GetPointIntersection(&empty, 0, from, to, out)) fail = 1;
    if (fail) { std::printf("008fbcb0_TEST FAIL\n"); return 1; }
    std::printf("008fbcb0_TEST PASS\n");
    return 0;
}