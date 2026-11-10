// Behaviour test for CResource::GetUnusedFrames (0x009FBF70): list frame minus last-used frame, or 0xFFFFFFFF with no list.
#include <stdio.h>
class CResourceList { public: unsigned int CurrentFrame; unsigned int GetCurrentFrame() const { return CurrentFrame; } };
class CResource { public:
    CResourceList* ResourceList; unsigned int ResourceSize; unsigned int LastUsedFrame;
    unsigned int GetUnusedFrames() const { if (ResourceList) return ResourceList->GetCurrentFrame() - LastUsedFrame; return 0xffffffffu; }
};
int main()
{
    CResourceList list; list.CurrentFrame = 50;
    CResource r; r.ResourceList = &list; r.ResourceSize = 0; r.LastUsedFrame = 20;
    if (r.GetUnusedFrames() != 30) { printf("009fbf70_TEST FAIL\n"); return 1; }
    r.ResourceList = 0;
    if (r.GetUnusedFrames() != 0xffffffffu) { printf("009fbf70_TEST FAIL\n"); return 1; }
    printf("009fbf70_TEST PASS\n");
    return 0;
}
