struct CActionTurnToFacePosition {
    void* vftable;
};

void __fastcall CActionTurnToFacePosition_dtor(CActionTurnToFacePosition* action);
void operator delete(void*);

void* __fastcall CActionTurnToFacePosition_scalar_deleting_destructor(CActionTurnToFacePosition* action, int edx_unused, unsigned int flags)
{
    CActionTurnToFacePosition_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}