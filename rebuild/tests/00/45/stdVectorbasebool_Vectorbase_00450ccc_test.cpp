// Behaviour test for 0x00450CCC std::_Vector_base<bool>::_Vector_base(n, a): allocate n bytes, finish = start, end = start + n
#include <cstdio>
#include <cstdlib>
template <class T> struct Alloc {
    Alloc() { }
    T* allocate(unsigned int count) { return count != 0 ? static_cast<T*>(std::malloc(count * sizeof(T))) : 0; }
};
template <class T> struct VectorBase {
    T* start; T* finish; T* end;
    VectorBase(unsigned int n, const Alloc<T>& a) : start(0), finish(0), end(0)
    {
        Alloc<T> al(a);
        start = al.allocate(n);
        finish = start;
        end = start + n;
    }
};
int main()
{
    Alloc<bool> a;
    VectorBase<bool> v(6, a);
    if (v.start == 0 || v.finish != v.start || v.end != v.start + 6) { std::printf("00450ccc_TEST FAIL\n"); return 1; }
    std::free(v.start);
    VectorBase<bool> z(0, a);
    if (z.start != 0 || z.finish != 0 || z.end != 0) { std::printf("00450ccc_TEST FAIL\n"); return 1; }
    std::printf("00450ccc_TEST PASS\n");
    return 0;
}