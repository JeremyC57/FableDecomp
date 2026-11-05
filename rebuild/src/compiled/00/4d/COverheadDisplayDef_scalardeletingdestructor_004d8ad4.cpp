// Leaf: ?COverheadDisplayDef_scalar_deleting_destructor@@YIPAXPAVCOverheadDisplayDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class COverheadDisplayDef
{ public:
    void* vftable;
};
extern void* const g_COverheadDisplayDef_vftable;
void operator delete(void*);
void* __fastcall COverheadDisplayDef_scalar_deleting_destructor(COverheadDisplayDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_COverheadDisplayDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}