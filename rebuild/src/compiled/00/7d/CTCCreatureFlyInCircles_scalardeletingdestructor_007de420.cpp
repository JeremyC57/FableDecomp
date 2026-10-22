struct CTCCreatureFlyInCircles {
    void* vftable;
};

void __fastcall CTCCreatureFlyInCircles_dtor(CTCCreatureFlyInCircles* circles);
void operator delete(void*);

void* __fastcall CTCCreatureFlyInCircles_scalar_deleting_destructor(CTCCreatureFlyInCircles* circles, int edx_unused, unsigned int flags)
{
    CTCCreatureFlyInCircles_dtor(circles);
    if (flags & 1)
        operator delete(circles);
    return circles;
}