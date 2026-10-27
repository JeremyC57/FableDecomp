// Behaviour test for 0x0086E9F0 std::vector<CBodyPartMesh>::resize(n, x): erase the tail when shrinking, otherwise
// fill-insert (n - size()) copies of x at the end.
#include <cstdio>

struct Mesh { long a, b; };

class MeshVector {
public:
    Mesh data[16];
    Mesh* start; Mesh* finish;
    MeshVector() : start(data), finish(data) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    Mesh* erase(Mesh* f, Mesh* l) { Mesh* d = f; for (Mesh* s = l; s != finish; ++s, ++d) *d = *s; finish = d; return f; }
    void fill_insert(Mesh* pos, unsigned int n, const Mesh& x)
    {
        for (Mesh* s = finish; s != pos; ) { --s; s[n] = *s; }
        for (unsigned int i = 0; i < n; ++i) pos[i] = x;
        finish += n;
    }
    void resize(unsigned int n, const Mesh& x)
    {
        if (n < size()) erase(start + n, finish);
        else fill_insert(finish, n - size(), x);
    }
};

int main()
{
    MeshVector v;
    Mesh x = { 1, 2 };
    v.resize(3, x);
    if (v.size() != 3 || v.start[2].a != 1 || v.start[2].b != 2) { std::printf("0086e9f0_TEST FAIL\n"); return 1; }
    Mesh y = { 5, 6 };
    v.resize(5, y);
    if (v.size() != 5 || v.start[2].a != 1 || v.start[3].a != 5 || v.start[4].b != 6) { std::printf("0086e9f0_TEST FAIL\n"); return 1; }
    v.resize(2, y);
    if (v.size() != 2 || v.start[1].a != 1) { std::printf("0086e9f0_TEST FAIL\n"); return 1; }
    v.resize(2, y);
    if (v.size() != 2) { std::printf("0086e9f0_TEST FAIL\n"); return 1; }
    std::printf("0086e9f0_TEST PASS\n");
    return 0;
}