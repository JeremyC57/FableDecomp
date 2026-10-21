#include <cstdio>
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
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
    CDefPointeeBase& operator=(const CDefPointeeBase& other);   // 0x00431F60 (the CResource member copy, out of line)
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    struct CSubDefInfo {
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
void AssignCDefClassBase(CDefClassBase& to, const CDefClassBase& from)               // emission helper, not a retail function
{
    to = from;
}


static int g_base;
CResource::~CResource() {}
CDefPointeeBase& CDefPointeeBase::operator=(const CDefPointeeBase& other) { ++g_base; IVRefCount = other.IVRefCount; LastUsedFrame = other.LastUsedFrame; return *this; }
int main()
{
    CDefClassBase a, b;
    a.IVRefCount = 1; a.LastUsedFrame = 2; a.PDefManager = (CDefinitionManager*)0x10; a.GlobalIndex = 77;
    a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1;
    unsigned char* pb = (unsigned char*)&b + 0x24; *pb = 0xf2;   // Template set, upper bits set
    AssignCDefClassBase(b, a);
    bool ok = g_base == 1 && b.IVRefCount == 1 && b.LastUsedFrame == 2 && b.PDefManager == (CDefinitionManager*)0x10 && b.GlobalIndex == 77
        && b.Setup == 1 && b.Template == 0 && b.DefaultValsApplied == 1 && (*pb & 0xf8) == 0xf0;
    if (ok) std::printf("00431f10_TEST PASS\n");
    return 0;
}