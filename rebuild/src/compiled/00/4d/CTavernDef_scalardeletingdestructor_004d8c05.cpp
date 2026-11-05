// Leaf: ?CTavernDef_scalar_deleting_destructor@@YIPAXPAVCTavernDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTavernDef
{ public:
    void* vftable;
};
extern void* const g_CTavernDef_vftable;
void operator delete(void*);
void* __fastcall CTavernDef_scalar_deleting_destructor(CTavernDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTavernDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}