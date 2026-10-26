// behaviour test for 0x00434240 CConfigOptionsDefaultsDef::CConfigOptionsDefaultsDef(): the def bases come up with IVRefCount 1, no definition manager, the three flag bits clear, no instantiation name (-1), an empty clean SubDefInfoMap, and the object's dynamic type is the derived def (its size slot answers 0x74)
#include <cstdio>
#include <cstring>
#include <new>
class CResource {
public:
    CResource() : IVRefCount(1), ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) { }
    virtual ~CResource() { }
    virtual unsigned int GetSize() const { return 0x1c; }
    unsigned int IVRefCount;
    void* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned int ResourceSize;
    unsigned int LastUsedFrame;
};
class CDefPointeeBase : public CResource { };
class CDefClassBase : public CDefPointeeBase {
public:
    CDefClassBase() : PDefManager(0) { Setup = false; Template = false; DefaultValsApplied = false; }
    void* PDefManager;
    unsigned int GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
struct CSubDefInfo { int DefIndex; int OriginalParentDefIndex; };
struct SubDefInfoMap { CSubDefInfo* First; CSubDefInfo* Last; CSubDefInfo* End; bool Dirty; SubDefInfoMap() : First(0), Last(0), End(0), Dirty(false) { } };
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase() : InstantiationName(-1) { }
    int InstantiationName;
    SubDefInfoMap Map;
};
class CDerivedDef : public CParentDefClassBase {             // the def under test (CConfigOptionsDefaultsDef)
public:
    CDerivedDef() { }
    virtual unsigned int GetSize() const { return 0x74; }
};
int main()
{
    int fail = 0;
    static double storage[64];
    std::memset(storage, 0xA5, sizeof(storage));
    CDerivedDef* d = new (storage) CDerivedDef();
    if (d->IVRefCount != 1 || d->ResourceList != 0 || d->PrevResource != 0 || d->NextResource != 0) fail |= 1;
    if (d->PDefManager != 0 || d->Setup || d->Template || d->DefaultValsApplied) fail |= 2;
    if (d->InstantiationName != -1) fail |= 4;
    if (d->Map.First != 0 || d->Map.Last != 0 || d->Map.End != 0 || d->Map.Dirty) fail |= 8;
    CResource* r = d;
    if (r->GetSize() != 0x74) fail |= 16;
    if (fail) { std::printf("00434240_TEST FAIL %d\n", fail); return 1; }
    std::printf("00434240_TEST PASS\n");
    return 0;
}