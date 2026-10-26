#include <cstdio>

class CLandscapeBackgroundPatch { public: void** vftable; };
void* g_CLandscapeBackgroundPatch_vftable;
int g_deleted = 0;
class __declspec(novtable) CResource {
public:
    virtual ~CResource();
};
CResource::~CResource()
{
    
}
void __cdecl operator delete(void* p)
{ (void)p; g_deleted = 1; }

extern void* __fastcall CDialogueLayerDef_vector_dtor(
    CLandscapeBackgroundPatch*, int, unsigned int);

int main() {
    CLandscapeBackgroundPatch obj; obj.vftable = 0;
    void* r = CDialogueLayerDef_vector_dtor(&obj, 0, 1u);
    if (r == &obj && obj.vftable == (void**)&g_CLandscapeBackgroundPatch_vftable && g_deleted)
        printf("OK_0046642a\n");
    return 0;
}