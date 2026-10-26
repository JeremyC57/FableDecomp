class CMovableResourceMemoryPool { public:
    void* vftable;
};

void __fastcall CMovableResourceMemoryPool_dtor(CMovableResourceMemoryPool* pool);
void operator delete(void*);

void* __fastcall CMovableResourceMemoryPool_scalar_deleting_destructor(CMovableResourceMemoryPool* pool, int edx_unused, unsigned int flags)
{
    CMovableResourceMemoryPool_dtor(pool);
    if (flags & 1)
        operator delete(pool);
    return pool;
}