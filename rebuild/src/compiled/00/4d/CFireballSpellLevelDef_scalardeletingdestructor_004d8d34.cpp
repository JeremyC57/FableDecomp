// Leaf: ?CFireballSpellLevelDef_scalar_deleting_destructor@@YIPAXPAVCFireballSpellLevelDef@@HI@Z
#pragma optimize("s", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CFireballSpellLevelDef
{ public:
    void* vftable;
};
extern void* const g_CFireballSpellLevelDef_vftable;
void operator delete(void*);
void* __fastcall CFireballSpellLevelDef_scalar_deleting_destructor(CFireballSpellLevelDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CFireballSpellLevelDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}