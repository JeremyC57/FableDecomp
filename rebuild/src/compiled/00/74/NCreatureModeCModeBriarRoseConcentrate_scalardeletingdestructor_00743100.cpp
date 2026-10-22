struct CModeBriarRoseConcentrate {
    void* vftable;
};

void __fastcall CModeBriarRoseConcentrate_dtor(CModeBriarRoseConcentrate* mode);
void operator delete(void*);

void* __fastcall CModeBriarRoseConcentrate_scalar_deleting_destructor(CModeBriarRoseConcentrate* mode, int edx_unused, unsigned int flags)
{
    CModeBriarRoseConcentrate_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}