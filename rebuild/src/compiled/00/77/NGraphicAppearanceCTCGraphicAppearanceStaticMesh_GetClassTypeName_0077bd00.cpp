// NGraphicAppearance::CTCGraphicAppearanceStaticMesh::GetClassTypeName @ 0x0077bd00
// __fastcall, returns CCharString by value.

class CCharString { public:
    char* p;
    int   len;
    CCharString(const char* s, int n);
};

struct CTCGraphicAppearanceStaticMesh {
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCGraphicAppearanceStaticMesh::GetClassTypeName()
{
    return CCharString((const char*)0x0123bc4c, -1);
}