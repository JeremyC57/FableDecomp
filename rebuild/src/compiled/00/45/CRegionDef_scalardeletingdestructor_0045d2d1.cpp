#pragma optimize("s", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern void __fastcall DestructMember_0045645d(void* member);
void* __fastcall CRegionDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_0045645d(static_cast<unsigned char*>(def) + 80);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}