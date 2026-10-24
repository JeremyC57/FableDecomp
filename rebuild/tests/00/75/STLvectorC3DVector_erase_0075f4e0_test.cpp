#include <cstdio>
struct C3DVector { float X, Y, Z; };
static C3DVector* copy_range(C3DVector* first, C3DVector* last, C3DVector* result)
{
    while (first != last) { *result = *first; ++result; ++first; }
    return result;
}
struct Vec3List {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* end() { return _M_finish; }
    C3DVector* erase(C3DVector* position)
    {
        if (position + 1 != end())
            copy_range(position + 1, _M_finish, position);
        --_M_finish;
        return position;
    }
};
int main()
{
    C3DVector buf[4];
    int i = 0;
    for (i = 0; i < 4; ++i) { buf[i].X = (float)i; buf[i].Y = 0.0f; buf[i].Z = 0.0f; }
    Vec3List v; v._M_start = buf; v._M_finish = buf + 4; v._M_end_of_storage = buf + 4;
    C3DVector* r = v.erase(buf + 1);
    if (r != buf + 1 || v._M_finish != buf + 3 || buf[1].X != 2.0f || buf[2].X != 3.0f) { std::printf("0075f4e0_TEST FAIL\n"); return 1; }
    r = v.erase(buf + 2);
    if (r != buf + 2 || v._M_finish != buf + 2 || buf[1].X != 2.0f) { std::printf("0075f4e0_TEST FAIL\n"); return 1; }
    std::printf("0075f4e0_TEST PASS\n");
    return 0;
}