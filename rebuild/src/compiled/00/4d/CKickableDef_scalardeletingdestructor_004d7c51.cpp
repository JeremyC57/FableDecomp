#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CKickableDef
{ public:
    void* vftable;
};
extern void* const g_CKickableDef_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CKickableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CKickableDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}