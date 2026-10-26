#pragma optimize("t", on)
class CScriptThing {
public:
    virtual ~CScriptThing();   // 0x004AA840
};
extern void __fastcall DestructFirstMember_0099eae0(void* member);
extern void __fastcall CActionPerformExpression_Destruct(void* action);
void* __fastcall CActionPerformExpression_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_0099eae0(static_cast<unsigned char*>(action) + 28);
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 12))->CScriptThing::~CScriptThing();
    CActionPerformExpression_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}