#pragma optimize("s",on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
struct CMember { };
extern void __fastcall CMember_dtor_004dd159(CMember* def);
extern const int g_vtbl;

class CHeroSuitDef { public:
    int f0;
    int Parent;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CHeroSuitDef_scalar_deleting_destructor(CHeroSuitDef* def, int edx, unsigned char flags)
{
    CMember_dtor_004dd159(&def->m28);
    *(const int**)def = &g_vtbl;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}