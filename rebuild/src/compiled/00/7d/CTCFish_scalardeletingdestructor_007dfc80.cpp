struct CTCFish {
    void* vftable;
};

void __fastcall CTCFish_dtor(CTCFish* fish);
void operator delete(void*);

void* __fastcall CTCFish_scalar_deleting_destructor(CTCFish* fish, int edx_unused, unsigned int flags)
{
    CTCFish_dtor(fish);
    if (flags & 1)
        operator delete(fish);
    return fish;
}