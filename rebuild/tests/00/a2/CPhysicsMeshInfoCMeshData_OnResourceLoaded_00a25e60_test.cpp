// Test for 0x00A25E60 CPhysicsMeshInfo::CMeshData::OnResourceLoaded: builds the mesh only when source data is present.
#include <stdio.h>
static int g_built = 0;
class CResource { public: virtual ~CResource() { } virtual void OnResourceLoaded() { } };
class CMeshData : public CResource {
public:
    virtual void OnResourceLoaded() { if (m_pSourceData) BuildMesh(); }
    void BuildMesh() { ++g_built; }
    void* m_pSourceData;
};
int main()
{
    CMeshData m; CResource* r = &m; int data;
    m.m_pSourceData = 0; r->OnResourceLoaded();
    bool ok = g_built == 0;
    m.m_pSourceData = &data; r->OnResourceLoaded();
    if (g_built != 1) ok = false;
    puts(ok ? "00a25e60_TEST PASS" : "00a25e60_TEST FAIL");
    return ok ? 0 : 1;
}