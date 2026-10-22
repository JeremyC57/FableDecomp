struct CModeBriarRoseTeleportInvulnerable {
    void* vftable;
};

void __fastcall CModeBriarRoseTeleportInvulnerable_dtor(CModeBriarRoseTeleportInvulnerable* mode);
void operator delete(void*);

void* __fastcall CModeBriarRoseTeleportInvulnerable_scalar_deleting_destructor(CModeBriarRoseTeleportInvulnerable* mode, int edx_unused, unsigned int flags)
{
    CModeBriarRoseTeleportInvulnerable_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}