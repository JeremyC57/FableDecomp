#include <cstdio>
#include <cstring>
#include <new>

// CParentDefClassBase::CParentDefClassBase(): CResource base, no manager, status bits clear, an unset
// instantiation name (-1) and an empty, clean sub-def vector map.
static int g_resourceCtors = 0;
struct CDefinitionManager;

struct CResource {
    virtual void v00() { }
    unsigned char _resource[0x18];
    CResource() { ++g_resourceCtors; }
};
struct CDefPointeeBase : CResource {
    virtual void v00() { }
    CDefPointeeBase() { }
};
struct CDefClassBase : CDefPointeeBase {
    CDefinitionManager* PDefManager;
    unsigned long       GlobalIndex;
    unsigned char       Setup : 1;
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
    virtual void v00() { }
    CDefClassBase() : PDefManager(0) { Setup = 0; Template = 0; DefaultValsApplied = 0; }
};
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};
struct CSubDefInfoVectorMap {
    void*         m_first;
    void*         m_last;
    void*         m_end;
    unsigned char Compare;
    bool          Dirty;
    CSubDefInfoVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
};
struct CParentDefClassBase : CDefClassBase {
    CDefString           InstantiationName;
    CSubDefInfoVectorMap SubDefInfoMap;
    virtual void v00() { }
    CParentDefClassBase();
};
CParentDefClassBase::CParentDefClassBase()
{
}

int main()
{
    static unsigned char storage[sizeof(CParentDefClassBase) + 16];
    std::memset(storage, 0xFF, sizeof(storage));
    CParentDefClassBase* d = new (storage) CParentDefClassBase();
    if (g_resourceCtors != 1 || d->PDefManager != 0 || d->Setup || d->Template || d->DefaultValsApplied
        || d->InstantiationName.TablePos != -1 || d->SubDefInfoMap.m_first != 0 || d->SubDefInfoMap.m_last != 0
        || d->SubDefInfoMap.m_end != 0 || d->SubDefInfoMap.Dirty != false || d->GlobalIndex != ~0UL) {
        std::printf("00430370_TEST FAIL\n");
        return 1;
    }
    std::printf("00430370_TEST PASS\n");
    return 0;
}
