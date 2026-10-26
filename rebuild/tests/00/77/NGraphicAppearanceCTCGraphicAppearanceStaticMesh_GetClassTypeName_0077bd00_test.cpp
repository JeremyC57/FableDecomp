#include <cstdio>
#include <cstring>

class CCharString { public:
    char* p;
    int   len;
    CCharString(const char* s, int n) : p((char*)s), len(n) {}
};

struct CTCGraphicAppearanceStaticMesh {
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCGraphicAppearanceStaticMesh::GetClassTypeName()
{
    return CCharString((const char*)0x0123bc4c, -1);
}

int main() {
    CTCGraphicAppearanceStaticMesh a; a.dummy = 7;
    CCharString r = a.GetClassTypeName();
    if (r.p == (char*)0x0123bc4c && r.len == -1) {
        printf("0077bd00_TEST PASS\n");
    } else {
        printf("FAIL %p %d\n", (void*)r.p, r.len);
    }
    return 0;
}