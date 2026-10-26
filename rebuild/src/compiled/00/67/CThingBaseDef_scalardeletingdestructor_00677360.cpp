#pragma optimize("t", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern void __fastcall DestructMember_00450ba0(void* member);
void* __fastcall CThingBaseDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00450ba0(static_cast<unsigned char*>(def) + 57);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}