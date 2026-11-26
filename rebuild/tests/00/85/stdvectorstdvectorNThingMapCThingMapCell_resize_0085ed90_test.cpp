#include <stdio.h>
namespace NThingMap { class CThingMapCell { public: int x; }; }
static unsigned int g_n=0;
static void* g_a=(void*)1; static void* g_b=(void*)1; static void* g_c=(void*)1;
namespace std {
template<class T> class allocator {};
template<class T, class Al=allocator<T> > class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void resize(unsigned int n);
    void resize(unsigned int n, const T& value);
};
template<class T, class Al>
void vector<T,Al>::resize(unsigned int n, const T& value) {
    g_n=n; g_a=value._M_start; g_b=value._M_finish; g_c=value._M_end_of_storage;
}
}
template void std::vector<std::vector<NThingMap::CThingMapCell> >::resize(unsigned int, const std::vector<NThingMap::CThingMapCell>&);
int main() {
    std::vector<std::vector<NThingMap::CThingMapCell> > v;
    v.resize(4);
    if (g_n==4 && g_a==0 && g_b==0 && g_c==0) { printf("0085ed90_TEST PASS\n"); return 0; }
    printf("0085ed90_TEST FAIL\n"); return 1;
}