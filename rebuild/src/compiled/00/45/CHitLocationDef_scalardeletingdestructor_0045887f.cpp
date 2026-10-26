#pragma optimize("s", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern void __fastcall DestructMember_0044f3b2(void* member);
void* __fastcall CHitLocationDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_0044f3b2(static_cast<unsigned char*>(def) + 60);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}