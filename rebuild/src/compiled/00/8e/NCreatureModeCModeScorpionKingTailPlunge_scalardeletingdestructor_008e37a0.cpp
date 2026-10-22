struct CModeScorpionKingTailPlunge {
    void* vftable;
};

void __fastcall CModeScorpionKingTailPlunge_dtor(CModeScorpionKingTailPlunge* mode);
void operator delete(void*);

void* __fastcall CModeScorpionKingTailPlunge_scalar_deleting_destructor(CModeScorpionKingTailPlunge* mode, int edx_unused, unsigned int flags)
{
    CModeScorpionKingTailPlunge_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}