// Behaviour test for 006ca650 CPointerHolder::CPointerHolder (stores the pointer)
#include <cstdio>
struct CPointerHolder {
    void* m_pointer;
    CPointerHolder(void* pointer);
};
CPointerHolder::CPointerHolder(void* pointer)
    : m_pointer(pointer)
{
}
int main()
{
    int x;
    CPointerHolder a(&x), b(0);
    if (a.m_pointer != &x || b.m_pointer != 0) { std::printf("006ca650_TEST FAIL\n"); return 1; }
    std::printf("006ca650_TEST PASS\n");
    return 0;
}
