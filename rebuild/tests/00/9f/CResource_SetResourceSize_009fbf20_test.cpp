// Behaviour test for CResource::SetResourceSize (0x009FBF20): the list total moves from the old size to the new one.
#include <stdio.h>
class CResourceList { public: long AllocatedMemory; };
class CResource { public:
    CResourceList* ResourceList; unsigned int ResourceSize;
    void SetResourceSize(unsigned int size)
    {
        CResourceList* list = ResourceList;
        if (list) { list->AllocatedMemory -= ResourceSize; ResourceList->AllocatedMemory += size; }
        ResourceSize = size;
    }
};
int main()
{
    CResourceList l; l.AllocatedMemory = 100;
    CResource r; r.ResourceList = &l; r.ResourceSize = 30;
    r.SetResourceSize(50);
    if (l.AllocatedMemory != 120 || r.ResourceSize != 50) { printf("009fbf20_TEST FAIL\n"); return 1; }
    r.ResourceList = 0; r.SetResourceSize(7);
    if (l.AllocatedMemory != 120 || r.ResourceSize != 7) { printf("009fbf20_TEST FAIL\n"); return 1; }
    printf("009fbf20_TEST PASS\n");
    return 0;
}
