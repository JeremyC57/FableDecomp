#pragma optimize("s",on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
extern void* const CPhysicsDef_vftable;

class CPhysicsDef { public:
    void* vftable;
};

void* __fastcall CPhysicsDef_scalar_deleting_destructor(CPhysicsDef* def, int edxpad, unsigned int flags)
{
    (void)edxpad;
    def->vftable = (void*)&CPhysicsDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}