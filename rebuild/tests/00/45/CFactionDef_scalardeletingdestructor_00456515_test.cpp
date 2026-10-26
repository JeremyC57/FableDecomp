#include <cstdio>
class CFactionDef { public:
    int f0;
    int Parent;
    void* f8;
    void CFactionDef_dtor();
    void* CFactionDef_scalar_deleting_destructor(unsigned int flags);
};
static int g_dtor = 0;
static int g_del = 0;
void CFactionDef::CFactionDef_dtor(){ g_dtor++; }
void __cdecl op_delete(void* p){ (void)p; g_del++; }
void* CFactionDef::CFactionDef_scalar_deleting_destructor(unsigned int flags)
{
    this->CFactionDef_dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}
int main(){
    CFactionDef obj;
    void* r = obj.CFactionDef_scalar_deleting_destructor(0);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL nodelete\n"); return 1; }
    r = obj.CFactionDef_scalar_deleting_destructor(1);
    if (r != &obj || g_dtor != 2 || g_del != 1){ std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456515_TEST PASS\n");
    return 0;
}