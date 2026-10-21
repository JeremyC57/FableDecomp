// test for 00431f60 CResource::operator=
#include <stdio.h>
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
void AssignCResource(CResource& to, const CResource& from)   // emission helper, not a retail function
{
    to = from;
}

CResource::~CResource() {}
struct CResA : CResource { };
struct CResB : CResource { virtual ~CResB() {} };
int main()
{
    CResA a; CResB b;
    a.IVRefCount = 1; a.ResourceList = (CResourceList*)2; a.PrevResource = (CResource*)3; a.NextResource = (CResource*)4; a.ResourceSize = 5; a.LastUsedFrame = 6;
    void* vb = *(void**)&b;
    AssignCResource(b, a);
    int ok = *(void**)&b == vb && b.IVRefCount == 1 && b.ResourceList == (CResourceList*)2 && b.PrevResource == (CResource*)3
          && b.NextResource == (CResource*)4 && b.ResourceSize == 5 && b.LastUsedFrame == 6;
    printf(ok ? "00431f60_TEST PASS\n" : "00431f60_TEST FAIL\n");
    return 0;
}