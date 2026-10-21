// test for 004304a0 ~CControlsDef
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
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
    ~vector() { if (_M_start) free(_M_start); }
};
}
template <class K, class V> struct CKeyPairCompareLess {};
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
class CActionInputControl {                              // element type does not reach the bytes; members descriptive
public:
    long Action;
    long Control;
};
class CControlsDef : public CParentDefClassBase {
public:
    std::vector<CActionInputControl> Controls;           // +0x3c
    bool ToggleZTarget;                                  // +0x48
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4a
    bool ToggleExpressionMenu;                           // +0x4b
    bool ToggleExpressionShift;                          // +0x4c
    bool FlourishNeedsAttackButtonHeld;                  // +0x4d
};
void DestroyCControlsDef(CControlsDef* p)                // emission helper, not a retail function
{
    p->CControlsDef::~CControlsDef();
}

static int g_res; static void* g_resvp;
CResource::~CResource() { ++g_res; g_resvp = *(void**)this; }
int main()
{
    static double buf[0x10];
    CControlsDef* p = (CControlsDef*)buf;
    p->Controls._M_start = (CActionInputControl*)malloc(16);
    p->SubDefInfoMap._M_start = (std::pair<unsigned long, CDefClassBase::CSubDefInfo>*)malloc(24);
    DestroyCControlsDef(p);
    int ok = g_res == 1 && g_resvp != 0 && sizeof(CControlsDef) == 0x50 && offsetof(CControlsDef, Controls) == 0x3c;
    p->Controls._M_start = 0; p->SubDefInfoMap._M_start = 0;
    DestroyCControlsDef(p);
    ok = ok && g_res == 2;
    printf(ok ? "004304a0_TEST PASS\n" : "004304a0_TEST FAIL\n");
    return 0;
}