#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CCarriedReadableDef
{ public:
    void* vftable;
};
extern void* const g_CCarriedReadableDef_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCarriedReadableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCarriedReadableDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}