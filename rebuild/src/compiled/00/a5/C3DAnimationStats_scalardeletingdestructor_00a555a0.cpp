struct C3DAnimationStats {
    void* vftable;
};

void __fastcall C3DAnimationStats_dtor(C3DAnimationStats* stats);
void operator delete(void*);

void* __fastcall C3DAnimationStats_scalar_deleting_destructor(C3DAnimationStats* stats, int edx_unused, unsigned int flags)
{
    C3DAnimationStats_dtor(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}