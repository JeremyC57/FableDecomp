#include <cstdio>
#include <cstring>
#include <new>

// CDefClassBase::CDefClassBase(): CResource base runs, the three status bits are cleared (the other
// bits of the byte are kept), the definition manager pointer is nulled, GlobalIndex is left alone.
static int g_resourceCtors = 0;
class CDefinitionManager;

class CResource { public:
    virtual void v00() { }
    unsigned char _resource[0x18];
    CResource() { ++g_resourceCtors; }
};
class CDefPointeeBase : public CResource { public:
    virtual void v00() { }
    CDefPointeeBase() { }
};
class CDefClassBase : public CDefPointeeBase { public:
    CDefinitionManager* PDefManager;
    unsigned long       GlobalIndex;
    unsigned char       Setup : 1;
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
    unsigned char       OtherBits : 5;
    virtual void v00() { }
    CDefClassBase();
};
CDefClassBase::CDefClassBase()
    : PDefManager(0)
{
    Setup = 0;
    Template = 0;
    DefaultValsApplied = 0;
}

int main()
{
    static unsigned char storage[sizeof(CDefClassBase) + 16];
    std::memset(storage, 0xFF, sizeof(storage));
    CDefClassBase* d = new (storage) CDefClassBase();
    if (g_resourceCtors != 1 || d->PDefManager != 0 || d->GlobalIndex != ~0UL
        || d->Setup != 0 || d->Template != 0 || d->DefaultValsApplied != 0 || d->OtherBits != 0x1F) {
        std::printf("0042d9e0_TEST FAIL\n");
        return 1;
    }
    std::printf("0042d9e0_TEST PASS\n");
    return 0;
}