#pragma optimize("t", on)
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};
void* __fastcall CCreatureAction_Read_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CWideString*)(static_cast<unsigned char*>(action) + 168))->~CWideString();
    ((CCreatureActionBase*)(action))->CCreatureActionBase::~CCreatureActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}