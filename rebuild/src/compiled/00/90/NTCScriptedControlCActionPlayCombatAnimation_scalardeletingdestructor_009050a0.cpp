#pragma optimize("t", on)
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
extern void __fastcall CActionPlayCombatAnimation_Destruct(void* action);
void* __fastcall CActionPlayCombatAnimation_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(action) + 16))->~CCharString();
    CActionPlayCombatAnimation_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}