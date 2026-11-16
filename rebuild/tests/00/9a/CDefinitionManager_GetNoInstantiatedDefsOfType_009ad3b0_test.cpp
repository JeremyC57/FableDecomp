// Behaviour test for 0x009AD3B0 CDefinitionManager::GetNumberOfDefsOfType
#include <stdio.h>
#include <string.h>
class CCharString { public: const char* text; };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
struct CDefClassInfo {
    const char*                  Name;
    void*                        Create;
    CRetailVector<unsigned long> Instances;
};
class CDefinitionManager { public:
    CDefClassInfo* known;
    CDefClassInfo* GetClassInfo(const CCharString& className) { return strcmp(className.text, known->Name) == 0 ? known : 0; }
    long GetNumberOfDefsOfType(const CCharString& typeName);
};
long CDefinitionManager::GetNumberOfDefsOfType(const CCharString& typeName)
{
    CDefClassInfo* info = GetClassInfo(typeName);
    if (info)
        return info->Instances.size();
    return 0;
}
int main()
{
    int ok = 1;
    unsigned long ids[5];
    CDefClassInfo info;
    info.Name = "CCreatureDef"; info.Create = 0;
    info.Instances._M_start = ids; info.Instances._M_finish = ids + 5; info.Instances._M_end_of_storage = ids + 5;
    CDefinitionManager mgr;
    mgr.known = &info;
    CCharString a; a.text = "CCreatureDef";
    CCharString b; b.text = "CMissing";
    if (mgr.GetNumberOfDefsOfType(a) != 5) ok = 0;
    if (mgr.GetNumberOfDefsOfType(b) != 0) ok = 0;
    printf(ok ? "009ad3b0_TEST PASS\n" : "009ad3b0_TEST FAIL\n");
    return ok ? 0 : 1;
}