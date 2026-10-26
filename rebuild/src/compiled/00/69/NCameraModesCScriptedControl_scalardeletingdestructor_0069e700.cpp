#pragma optimize("t", on)
extern void __fastcall DestructMember_005ed2c0(void* member);
namespace NCameraModes {
struct CBase {                                         // RTTI base of every camera mode
    virtual ~CBase();                                  // 0x0056E0E0 (Ego_r ??1CBase@NCameraModes@@UAE@XZ)
};
}
void* __fastcall CScriptedControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    DestructMember_005ed2c0(static_cast<unsigned char*>(control) + 164);
    ((NCameraModes::CBase*)control)->NCameraModes::CBase::~CBase();   // the base destructor (namepass CAM2)
    if (flags & 1)
        operator delete(control);
    return control;
}