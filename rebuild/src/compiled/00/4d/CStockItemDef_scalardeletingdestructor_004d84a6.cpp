#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CStockItemDef
{ public:
    void* vftable;
};
extern void* const g_CStockItemDef_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CStockItemDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CStockItemDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}