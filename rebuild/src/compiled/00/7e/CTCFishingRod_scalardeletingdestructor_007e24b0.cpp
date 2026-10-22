struct CTCFishingRod {
    void* vftable;
};

void __fastcall CTCFishingRod_dtor(CTCFishingRod* rod);
void operator delete(void*);

void* __fastcall CTCFishingRod_scalar_deleting_destructor(CTCFishingRod* rod, int edx_unused, unsigned int flags)
{
    CTCFishingRod_dtor(rod);
    if (flags & 1)
        operator delete(rod);
    return rod;
}