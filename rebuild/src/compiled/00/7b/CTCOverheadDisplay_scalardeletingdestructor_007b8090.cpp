struct CTCOverheadDisplay {
    void* vftable;
};

void __fastcall CTCOverheadDisplay_dtor(CTCOverheadDisplay* display);
void operator delete(void*);

void* __fastcall CTCOverheadDisplay_scalar_deleting_destructor(CTCOverheadDisplay* display, int edx_unused, unsigned int flags)
{
    CTCOverheadDisplay_dtor(display);
    if (flags & 1)
        operator delete(display);
    return display;
}