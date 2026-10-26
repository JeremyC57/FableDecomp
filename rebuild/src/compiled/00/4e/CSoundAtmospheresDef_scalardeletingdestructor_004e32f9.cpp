#pragma optimize("s",on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
struct CMember { };
extern void __fastcall CMember_dtor_00414ea0(CMember* def);
extern const int g_vtbl;

class CSoundAtmospheresDef { public:
    int f0;
    int Parent;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CSoundAtmospheresDef_scalar_deleting_destructor(CSoundAtmospheresDef* def, int edx, unsigned char flags)
{
    CMember_dtor_00414ea0(&def->m28);
    *(const int**)def = &g_vtbl;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1)
        operator delete(def);
    return def;
}