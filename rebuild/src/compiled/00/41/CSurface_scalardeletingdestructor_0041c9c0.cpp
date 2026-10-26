class CSurface { public: void* vftable; 
public:
    void Uninitialise();   // 0x009F2E20
};
extern const void* CSurface_vftable;
void* __fastcall CSurface_scalar_deleting_destructor(CSurface* surface, int _edx, unsigned int flags)
{
    surface->vftable = (void*)&CSurface_vftable;
    ((CSurface*)(surface))->Uninitialise();
    if (flags & 1) {
        operator delete(surface);
    }
    return surface;
}