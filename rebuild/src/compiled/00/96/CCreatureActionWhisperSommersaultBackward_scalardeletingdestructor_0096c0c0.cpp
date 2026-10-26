class CCreatureAction_WhisperSommersaultBackward { public:
    void* vftable;
};

void __fastcall CCreatureAction_WhisperSommersaultBackward_dtor(CCreatureAction_WhisperSommersaultBackward* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WhisperSommersaultBackward_scalar_deleting_destructor(CCreatureAction_WhisperSommersaultBackward* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WhisperSommersaultBackward_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}