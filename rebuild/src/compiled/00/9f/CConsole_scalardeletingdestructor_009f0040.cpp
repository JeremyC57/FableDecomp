class CConsole { public:
    void* vftable;
};

void __fastcall CConsole_dtor(CConsole* console);
void operator delete(void*);

void* __fastcall CConsole_scalar_deleting_destructor(CConsole* console, int edx_unused, unsigned int flags)
{
    CConsole_dtor(console);
    if (flags & 1)
        operator delete(console);
    return console;
}