// test for 00431ed0 CParentDefClassBase::operator=
#include <stdio.h>
#include <stddef.h>
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
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo { public:
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
class CDefString {
public:
    long TablePos;
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector<T>& operator=(const vector<T>& x);            // 0x00431F90 for pair<unsigned long, CSubDefInfo>
};
}
template <class K, class V> class CKeyPairCompareLess { public:};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                        // +0x0c
    bool Dirty;                                          // +0x0d
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
void AssignCParentDefClassBase(CParentDefClassBase& to, const CParentDefClassBase& from)   // emission helper, not a retail function
{
    to = from;
}

CResource::~CResource() {}
static int g_vec; static const void* g_vecfrom; static void* g_vecthis;
namespace std {
template <class T> vector<T>& vector<T>::operator=(const vector<T>& x) { ++g_vec; g_vecfrom = &x; g_vecthis = this; _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage; return *this; }
}
int main()
{
    CParentDefClassBase a, b;
    a.IVRefCount = 7; a.PDefManager = (CDefinitionManager*)8; a.GlobalIndex = 9; a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1;
    a.InstantiationName.TablePos = 42; a.SubDefInfoMap.Dirty = true; a.SubDefInfoMap._M_start = 0; a.SubDefInfoMap._M_finish = 0; a.SubDefInfoMap._M_end_of_storage = 0;
    b.Setup = 0; b.Template = 1; b.DefaultValsApplied = 0; b.SubDefInfoMap.Dirty = false;
    void* vb = *(void**)&b;
    AssignCParentDefClassBase(b, a);
    int ok = sizeof(CParentDefClassBase) == 0x39 && offsetof(CParentDefClassBase, InstantiationName) == 0x25 && offsetof(CParentDefClassBase, SubDefInfoMap) == 0x29
          && *(void**)&b == vb && b.IVRefCount == 7 && b.PDefManager == (CDefinitionManager*)8 && b.GlobalIndex == 9
          && b.Setup == 1 && b.Template == 0 && b.DefaultValsApplied == 1 && b.InstantiationName.TablePos == 42 && b.SubDefInfoMap.Dirty
          && g_vec == 1 && g_vecfrom == &a.SubDefInfoMap && g_vecthis == &b.SubDefInfoMap;
    printf(ok ? "00431ed0_TEST PASS\n" : "00431ed0_TEST FAIL\n");
    return 0;
}