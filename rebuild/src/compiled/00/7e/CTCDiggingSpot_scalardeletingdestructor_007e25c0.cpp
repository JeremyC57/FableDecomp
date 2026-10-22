struct CTCDiggingSpot {
    void* vftable;
};

void __fastcall CTCDiggingSpot_dtor(CTCDiggingSpot* spot);
void operator delete(void*);

void* __fastcall CTCDiggingSpot_scalar_deleting_destructor(CTCDiggingSpot* spot, int edx_unused, unsigned int flags)
{
    CTCDiggingSpot_dtor(spot);
    if (flags & 1)
        operator delete(spot);
    return spot;
}