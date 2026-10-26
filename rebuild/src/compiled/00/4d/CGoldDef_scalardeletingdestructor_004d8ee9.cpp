#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CGoldDef
{ public:
    void* vftable;
};
extern void* const g_CGoldDef_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CGoldDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CGoldDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}