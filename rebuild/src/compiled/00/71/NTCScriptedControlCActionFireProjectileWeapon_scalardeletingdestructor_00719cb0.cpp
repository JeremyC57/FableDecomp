#pragma optimize("t", on)
class CScriptThing {
public:
    virtual ~CScriptThing();   // 0x004AA840
};
extern void __fastcall DestructFirstMember_0066a1c0(void* member);
extern void __fastcall CActionFireProjectileWeapon_Destruct(void* action);
void* __fastcall CActionFireProjectileWeapon_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_0066a1c0(static_cast<unsigned char*>(action) + 24);
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 12))->CScriptThing::~CScriptThing();
    CActionFireProjectileWeapon_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}