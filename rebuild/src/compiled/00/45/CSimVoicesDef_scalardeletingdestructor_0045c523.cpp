#pragma optimize("s", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern void __fastcall DestructMember_00455e59(void* member);
void* __fastcall CSimVoicesDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00455e59(static_cast<unsigned char*>(def) + 60);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}