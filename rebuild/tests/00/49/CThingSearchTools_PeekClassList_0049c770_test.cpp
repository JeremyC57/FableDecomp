// Behaviour test for CThingSearchTools::PeekClassList @ 0x0049C770: returns the thing manager's class list (+0x18).
#include <stdio.h>
class CClass;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public: void* _M_node; };
}
class CWorldMap;
class CThingMapManager;
class CThingManager {
public:
    unsigned char _pad_0x00[0x18];
    std::list<CClass*> ClassList;
};
class CThingSearchTools {
public:
    CWorldMap*        WorldMap;
    CThingMapManager* PThingMapManager;
    CThingManager*    ThingManager;
    const std::list<CClass*>& Peek() const { return PeekClassList(); }
protected:
    const std::list<CClass*>& PeekClassList() const;
};
const std::list<CClass*>& CThingSearchTools::PeekClassList() const
{
    return ThingManager->ClassList;
}
int main()
{
    CThingManager manager;
    CThingSearchTools tools;
    tools.WorldMap = 0; tools.PThingMapManager = 0; tools.ThingManager = &manager;
    const std::list<CClass*>& list = tools.Peek();
    bool ok = (const char*)&list == (const char*)&manager + 0x18 && &list == &manager.ClassList;
    if (!ok) { printf("0049c770_TEST FAIL\n"); return 1; }
    printf("0049c770_TEST PASS\n");
    return 0;
}
