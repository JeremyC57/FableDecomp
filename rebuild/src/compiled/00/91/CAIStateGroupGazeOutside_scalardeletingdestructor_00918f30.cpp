struct CAIStateGroup_GazeOutside {
    void* vftable;
};

void __fastcall CAIStateGroup_GazeOutside_dtor(CAIStateGroup_GazeOutside* stateGroup);
void operator delete(void*);

void* __fastcall CAIStateGroup_GazeOutside_scalar_deleting_destructor(CAIStateGroup_GazeOutside* stateGroup, int edx_unused, unsigned int flags)
{
    CAIStateGroup_GazeOutside_dtor(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}