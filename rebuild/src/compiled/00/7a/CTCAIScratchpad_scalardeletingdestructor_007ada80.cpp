struct CTCAIScratchpad {
    void* vftable;
};

void __fastcall CTCAIScratchpad_dtor(CTCAIScratchpad* scratchpad);
void operator delete(void*);

void* __fastcall CTCAIScratchpad_scalar_deleting_destructor(CTCAIScratchpad* scratchpad, int edx_unused, unsigned int flags)
{
    CTCAIScratchpad_dtor(scratchpad);
    if (flags & 1)
        operator delete(scratchpad);
    return scratchpad;
}