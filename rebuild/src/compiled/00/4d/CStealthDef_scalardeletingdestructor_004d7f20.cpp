// Leaf: ?CStealthDef_scalar_deleting_destructor@@YIPAXPAVCStealthDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CStealthDef
{ public:
    void* vftable;
};
extern void* const g_CStealthDef_vftable;
void operator delete(void*);
void* __fastcall CStealthDef_scalar_deleting_destructor(CStealthDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CStealthDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}