#pragma optimize("s",on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
extern const void* CThingMultiArrowShotDef_vftable;

class CThingMultiArrowShotDef { public:
    const void** vft;
};


void* __fastcall CThingMultiArrowShotDef_scalar_deleting_destructor(CThingMultiArrowShotDef* def, int edx, unsigned int flags) {
    (void)edx;
    def->vft = (const void**)&CThingMultiArrowShotDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}