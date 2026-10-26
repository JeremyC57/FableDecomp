class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
struct Member;
// Leaf: ??1CAIStateGroup_HoverWanderAroundThing@
extern "C" void __fastcall Member_dtor_00453e1a(Member* m);
struct Base;
extern void* const g_vtbl[1];
struct CAIStateGroup_HoverWanderAroundThing {
    void* vt;
    char pad[0x24];
    Member* m28;
};
void __fastcall CAIStateGroup_HoverWanderAroundThing_dtor(CAIStateGroup_HoverWanderAroundThing* self)
{
    Member_dtor_00453e1a((Member*)((char*)self + 0x28));
    self->vt = (void*)g_vtbl;
    ((CResource*)((Base*)self))->CResource::~CResource();
}