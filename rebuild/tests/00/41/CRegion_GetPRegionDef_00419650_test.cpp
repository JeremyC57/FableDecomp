// behaviour test for 0x00419650 CRegion::GetPRegionDef
#include <stdio.h>
class CDefPointeeBase {
public:
    void* __vftable;
    long  RefCount;                                      // +0x04
};
class CRegionDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer() {}
    CDefPointer() : Object(0) {}
};
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CRegion {
public:
    CRetailVector<long>            Maps;                 // +0x00
    CRetailVector<long>            VisMaps;              // +0x0c
    CCharString                    Name;                 // +0x18
    CCharString                    DisplayName;          // +0x1c
    CCharString                    RegionDefName;        // +0x20
    CDefPointer<const CRegionDef>  PDef;                 // +0x24
    CDefPointer<const CRegionDef> GetPRegionDef() const;
};

CDefPointer<const CRegionDef> CRegion::GetPRegionDef() const
{
    return PDef;
}
int main()
{
    CDefPointeeBase def; def.RefCount = 1;
    CRegion r; r.PDef.Object = &def;
    int ok = 1;
    {
        CDefPointer<const CRegionDef> p = r.GetPRegionDef();
        ok &= p.Object == &def && def.RefCount == 2;
    }
    r.PDef.Object = 0;
    {
        CDefPointer<const CRegionDef> q = r.GetPRegionDef();
        ok &= q.Object == 0 && def.RefCount == 2;
    }
    printf(ok ? "00419650_TEST PASS\n" : "00419650_TEST FAIL\n");
    return ok ? 0 : 1;
}