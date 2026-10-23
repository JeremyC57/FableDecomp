#pragma optimize("s",on)
struct CTCEnemy { void* vtbl; int ParentThing; };

extern void __stdcall SF_helper_0066ec8a(int a, int b);
extern int __fastcall SF_make_0066ec31(void* obj, int p1, int p2);

CTCEnemy* __fastcall CTCEnemy_SetFaction(CTCEnemy* self, int edx_unused, int p1, int p2, int p3)
{
    SF_helper_0066ec8a(p1, p3);
    void* vt = *(void**)self;
    self->ParentThing = SF_make_0066ec31(vt, p1, p2);
    return self;
}