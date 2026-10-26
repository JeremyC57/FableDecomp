#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CCrateStackDef
{ public:
    void* vftable;
};
extern void* const g_CCrateStackDef_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCrateStackDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCrateStackDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}