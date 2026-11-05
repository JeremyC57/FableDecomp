// Leaf: ?CLookDef_scalar_deleting_destructor@@YIPAXPAVCLookDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CLookDef
{ public:
    void* vftable;
};
extern void* const g_CLookDef_vftable;
void operator delete(void*);
void* __fastcall CLookDef_scalar_deleting_destructor(CLookDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CLookDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}