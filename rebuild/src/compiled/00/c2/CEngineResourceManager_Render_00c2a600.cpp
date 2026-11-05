// Leaf: ?CEngineResourceManager_Render@@YIXPAVCEngineResourceManager@@HPAUParam@@@Z
struct Param;

void __fastcall Param_method(Param* self);         // 0xc37930
void operator delete(void*);   // 0xbfe9bc

struct Param {
    char pad[0x10];
    int refcount;   // +0x10
};

class CEngineResourceManager { public: char x; };

void __fastcall CEngineResourceManager_Render(CEngineResourceManager* self, int edx, Param* p)
{
    (void)self;
    if (--p->refcount == 0) {
        Param_method(p);
        operator delete(p);
    }
}