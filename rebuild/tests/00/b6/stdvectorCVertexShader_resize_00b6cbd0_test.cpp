// Behaviour test for 0x00B6CBD0 vector<CVertexShader>::resize(n): shrinking erases [begin()+n, end()), growing fill-inserts n-size() copies of a default shader at end(); the default temporary is destroyed either way.
#include <cstdio>
static int g_erase = 0, g_fill = 0, g_ctor = 0, g_dtor = 0;
static unsigned int g_erase_from = 0, g_fill_n = 0;
struct Shader {
    void* data;
    Shader() : data(0) { ++g_ctor; }
    ~Shader() { ++g_dtor; }
};
struct Vec {
    Shader* start; Shader* finish;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void erase(Shader* first, Shader* last) { ++g_erase; g_erase_from = (unsigned int)(first - start); finish = first; (void)last; }
    void fill_insert(Shader* pos, unsigned int n, const Shader&) { ++g_fill; g_fill_n = n; finish = pos + n; }
    void resize(unsigned int n)
    {
        Shader x;
        if (n < size()) erase(start + n, finish);
        else fill_insert(finish, n - size(), x);
    }
};
int main()
{
    static Shader storage[8];
    g_ctor = 0;
    Vec v; v.start = storage; v.finish = storage + 5;
    v.resize(2);
    if (g_erase != 1 || g_erase_from != 2 || v.size() != 2 || g_fill != 0 || g_ctor != 1 || g_dtor != 1) { std::printf("00b6cbd0_TEST FAIL\n"); return 1; }
    v.resize(6);
    if (g_fill != 1 || g_fill_n != 4 || v.size() != 6 || g_erase != 1 || g_ctor != 2 || g_dtor != 2) { std::printf("00b6cbd0_TEST FAIL\n"); return 1; }
    v.resize(6);                                       // equal size: an empty fill-insert
    if (g_fill != 2 || g_fill_n != 0 || v.size() != 6) { std::printf("00b6cbd0_TEST FAIL\n"); return 1; }
    std::printf("00b6cbd0_TEST PASS\n");
    return 0;
}