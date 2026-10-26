void operator delete[](void*);

class CLocalDetailCacheMap { public:
    char pad[0x10];
    void* f10;
};

void __fastcall CloseStaticMap(CLocalDetailCacheMap* self)
{
    operator delete[](self->f10);
    self->f10 = 0;
}