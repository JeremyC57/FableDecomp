// Test for 0x00A35B10 C3DMeshObject::GetHelperPointNamesStartingWith: forwards to the shared mesh stats with sort = false.
#include <stdio.h>
static int g_calls = 0, g_sort = -1; static const char* g_prefix = 0;
class C3DMeshStats { public: void GetHelperPointNamesStartingWith(int* names, const char* prefix, bool sort) const { ++g_calls; g_prefix = prefix; g_sort = sort ? 1 : 0; *names = 7; } };
template <class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
class C3DMeshObject {
public:
    void GetHelperPointNamesStartingWith(int* names, const char* prefix) const { Mesh->GetHelperPointNamesStartingWith(names, prefix, false); }
    void* MeshInfo; CCountedPointer<const C3DMeshStats> Mesh;
};
int main()
{
    C3DMeshStats st; C3DMeshObject o; o.Mesh.Data = &st; int names = 0;
    o.GetHelperPointNamesStartingWith(&names, "HDL");
    bool ok = g_calls == 1 && g_sort == 0 && g_prefix[0] == 'H' && names == 7;
    puts(ok ? "00a35b10_TEST PASS" : "00a35b10_TEST FAIL");
    return ok ? 0 : 1;
}