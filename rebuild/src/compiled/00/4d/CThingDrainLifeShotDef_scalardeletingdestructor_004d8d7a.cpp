// Leaf: ?CThingDrainLifeShotDef_scalar_deleting_destructor@@YIPAXPAVCThingDrainLifeShotDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CThingDrainLifeShotDef
{ public:
    void* vftable;
};
extern void* const g_CThingDrainLifeShotDef_vftable;
void operator delete(void*);
void* __fastcall CThingDrainLifeShotDef_scalar_deleting_destructor(CThingDrainLifeShotDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CThingDrainLifeShotDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}