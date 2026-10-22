struct CAIStateGroup_GazeAtHome {
    void* vftable;
};

void __fastcall CAIStateGroup_GazeAtHome_dtor(CAIStateGroup_GazeAtHome* stateGroup);
void operator delete(void*);

void* __fastcall CAIStateGroup_GazeAtHome_scalar_deleting_destructor(CAIStateGroup_GazeAtHome* stateGroup, int edx_unused, unsigned int flags)
{
    CAIStateGroup_GazeAtHome_dtor(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}