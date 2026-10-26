#pragma optimize("s", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern void __fastcall DestructMember_00440d60(void* member);
void* __fastcall CCardPositionsDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00440d60(static_cast<unsigned char*>(def) + 60);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}