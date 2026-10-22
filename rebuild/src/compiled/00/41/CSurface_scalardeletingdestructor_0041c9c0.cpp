extern void __cdecl operator_delete_stub(void*);
struct CSurface { void* vftable; };
extern const void* CSurface_vftable;
extern void __fastcall CSurface_dtor(CSurface* surface);
void* __fastcall CSurface_scalar_deleting_destructor(CSurface* surface, int _edx, unsigned int flags)
{
    surface->vftable = (void*)&CSurface_vftable;
    CSurface_dtor(surface);
    if (flags & 1) {
        operator_delete_stub(surface);
    }
    return surface;
}