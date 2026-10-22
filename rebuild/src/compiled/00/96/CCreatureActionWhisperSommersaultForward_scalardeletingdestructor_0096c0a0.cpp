struct CCreatureAction_WhisperSommersaultForward {
    void* vftable;
};

void __fastcall CCreatureAction_WhisperSommersaultForward_dtor(CCreatureAction_WhisperSommersaultForward* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WhisperSommersaultForward_scalar_deleting_destructor(CCreatureAction_WhisperSommersaultForward* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WhisperSommersaultForward_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}