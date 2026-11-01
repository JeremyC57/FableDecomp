// Behaviour test for CClassFactory::AddNewClassType (0x00A36F90): the slot of the init's TypeID is marked used and receives
// the Local flag, the allocation function and the name.
#include <cstdio>
#include <cstring>
struct Class;
static Class* Alloc() { return 0; }
struct ClassTypeInfo { void* TypeList; unsigned int NoAllocated; bool Used; bool Local; Class* (*AllocFunc)(); const char* Name; };
struct ClassTypeInit { unsigned char TypeID; Class* (*AllocFunc)(); bool Local; const char* Name; };
struct ClassFactory {
    ClassTypeInfo* start;
    void AddNewClassType(const ClassTypeInit& init)
    {
        ClassTypeInfo& info = start[init.TypeID];
        info.Used = true;
        info.Local = init.Local;
        info.AllocFunc = init.AllocFunc;
        info.Name = init.Name;
    }
};
int main()
{
    ClassTypeInfo infos[4];
    std::memset(infos, 0, sizeof(infos));
    ClassFactory f; f.start = infos;
    ClassTypeInit init; init.TypeID = 2; init.AllocFunc = Alloc; init.Local = true; init.Name = "CThing";
    f.AddNewClassType(init);
    if (!infos[2].Used || !infos[2].Local || infos[2].AllocFunc != Alloc || std::strcmp(infos[2].Name, "CThing") != 0 || infos[1].Used || infos[3].Used) {
        std::printf("00a36f90_TEST FAIL\n"); return 1;
    }
    std::printf("00a36f90_TEST PASS\n");
    return 0;
}