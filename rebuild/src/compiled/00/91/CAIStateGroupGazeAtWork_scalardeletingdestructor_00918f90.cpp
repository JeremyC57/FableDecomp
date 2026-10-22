struct CAIStateGroup_GazeAtWork {
    void* vftable;
};

void __fastcall CAIStateGroup_GazeAtWork_dtor(CAIStateGroup_GazeAtWork* stateGroup);
void operator delete(void*);

void* __fastcall CAIStateGroup_GazeAtWork_scalar_deleting_destructor(CAIStateGroup_GazeAtWork* stateGroup, int edx_unused, unsigned int flags)
{
    CAIStateGroup_GazeAtWork_dtor(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}