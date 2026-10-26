extern void __cdecl engine_operator_delete(void* p);

class CPlayerManager { public:
    void* vftable;
    void RealDtor();
    void* CPlayerManager_scalar_deleting_destructor(unsigned int flags);
};

void* CPlayerManager::CPlayerManager_scalar_deleting_destructor(unsigned int flags)
{
    this->RealDtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}