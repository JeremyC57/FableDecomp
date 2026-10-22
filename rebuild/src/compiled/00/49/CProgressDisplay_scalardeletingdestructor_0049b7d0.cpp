struct CProgressDisplay {
    void* vftable;
};

void __fastcall CProgressDisplay_dtor(CProgressDisplay* display);
void operator delete(void*);

void* __fastcall CProgressDisplay_scalar_deleting_destructor(CProgressDisplay* display, int edx_unused, unsigned int flags)
{
    CProgressDisplay_dtor(display);
    if (flags & 1)
        operator delete(display);
    return display;
}