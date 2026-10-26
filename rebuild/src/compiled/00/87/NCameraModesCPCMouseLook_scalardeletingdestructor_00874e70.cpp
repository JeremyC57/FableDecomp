#pragma optimize("t", on)
extern void __fastcall DestructMember_00874cc0(void* member);
namespace NCameraModes {
struct CBase {                                         // RTTI base of every camera mode
    virtual ~CBase();                                  // 0x0056E0E0 (Ego_r ??1CBase@NCameraModes@@UAE@XZ)
};
}
void* __fastcall CPCMouseLook_CompositeScalarDeletingDestructor(void* look, int, unsigned int flags)
{
    DestructMember_00874cc0(static_cast<unsigned char*>(look) + 164);
    ((NCameraModes::CBase*)look)->NCameraModes::CBase::~CBase();   // the base destructor (namepass CAM2)
    if (flags & 1)
        operator delete(look);
    return look;
}