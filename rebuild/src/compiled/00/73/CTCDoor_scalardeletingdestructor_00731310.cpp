struct CTCDoor {
    void* vftable;
};

void __fastcall CTCDoor_dtor(CTCDoor* door);
void operator delete(void*);

void* __fastcall CTCDoor_scalar_deleting_destructor(CTCDoor* door, int edx_unused, unsigned int flags)
{
    CTCDoor_dtor(door);
    if (flags & 1)
        operator delete(door);
    return door;
}