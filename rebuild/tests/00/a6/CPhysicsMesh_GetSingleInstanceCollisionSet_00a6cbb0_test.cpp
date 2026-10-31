// behaviour test for 00a6cbb0 CPhysicsMesh::GetSingleInstanceCollisionSet() const (same body as src/)
#include <cstdio>
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear() { _M_finish = _M_start; }
};
}
class CPhysicsMesh { public:
    static std::vector<long> SingleInstanceCollisionSet;
    std::vector<long>& GetSingleInstanceCollisionSet() const;
};
std::vector<long> CPhysicsMesh::SingleInstanceCollisionSet;
std::vector<long>& CPhysicsMesh::GetSingleInstanceCollisionSet() const
{
    SingleInstanceCollisionSet.clear();
    return SingleInstanceCollisionSet;
}
int main()
{
    long store[4] = { 1, 2, 3, 4 };
    CPhysicsMesh::SingleInstanceCollisionSet._M_start = store;
    CPhysicsMesh::SingleInstanceCollisionSet._M_finish = store + 3;
    CPhysicsMesh::SingleInstanceCollisionSet._M_end_of_storage = store + 4;
    CPhysicsMesh mesh;
    std::vector<long>& r = mesh.GetSingleInstanceCollisionSet();
    if (&r != &CPhysicsMesh::SingleInstanceCollisionSet || r._M_finish != store || r._M_start != store || r._M_end_of_storage != store + 4) {
        std::printf("00a6cbb0_TEST FAIL\n"); return 1;
    }
    std::printf("00a6cbb0_TEST PASS\n");
    return 0;
}
