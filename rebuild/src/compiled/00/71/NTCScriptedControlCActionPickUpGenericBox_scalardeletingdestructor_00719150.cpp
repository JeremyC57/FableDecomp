#pragma optimize("t", on)
class CScriptThing {
public:
    virtual ~CScriptThing();   // 0x004AA840
};
extern void __fastcall CActionPickUpGenericBox_Destruct(void* action);
void* __fastcall CActionPickUpGenericBox_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 12))->CScriptThing::~CScriptThing();
    CActionPickUpGenericBox_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}