#include <cstdio>
struct CComp;
struct CP { CComp* m_pObject; void* m_pCount; };
struct CComp {
    virtual long GetType() { return type; }
    virtual CComp* FindFirstChildOfType(long t) { if (GetType() == t) return this; for (unsigned long i = 0; i < (unsigned long)(last - first); ++i) { CComp* f = first[i].m_pObject->FindFirstChildOfType(t); if (f) return f; } return 0; }
    long type; CP* first; CP* last;
};
int main()
{
    CComp root, a, b, c; root.type = 1; a.type = 2; b.type = 3; c.type = 3;
    CP kids[2] = { { &a, 0 }, { &b, 0 } }; CP akids[1] = { { &c, 0 } };
    root.first = kids; root.last = kids + 2; a.first = akids; a.last = akids + 1; b.first = b.last = 0; c.first = c.last = 0;
    if (root.FindFirstChildOfType(1) != &root || root.FindFirstChildOfType(3) != &c || root.FindFirstChildOfType(2) != &a || root.FindFirstChildOfType(9) != 0) { std::printf("00530460_TEST FAIL\n"); return 1; }
    std::printf("00530460_TEST PASS\n");
    return 0;
}
