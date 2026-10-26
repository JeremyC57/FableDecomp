#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CSkyDef;
extern void __fastcall Sub_Dtor_00453327(void* p);

void* __fastcall CSkyDef_scalar_deleting_destructor(CSkyDef* def, int edx, unsigned int flags)
{
    Sub_Dtor_00453327((char*)def + 0x74);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}