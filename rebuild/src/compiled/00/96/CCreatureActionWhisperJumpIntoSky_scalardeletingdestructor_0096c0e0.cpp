class CCreatureAction_WhisperJumpIntoSky { public:
    void* vftable;
};

void __fastcall CCreatureAction_WhisperJumpIntoSky_dtor(CCreatureAction_WhisperJumpIntoSky* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WhisperJumpIntoSky_scalar_deleting_destructor(CCreatureAction_WhisperJumpIntoSky* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WhisperJumpIntoSky_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}