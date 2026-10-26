#pragma optimize("s",on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CHairCardDef { public:
    void* vptr;
};
extern void* const CHairCardDef_vftable;

void* __fastcall CHairCardDef_scalar_deleting_destructor(CHairCardDef* def, int /*edx*/, unsigned int flags)
{
    def->vptr = (void*)&CHairCardDef_vftable;
    ((CResource*)(def))->CResource::~CResource();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}