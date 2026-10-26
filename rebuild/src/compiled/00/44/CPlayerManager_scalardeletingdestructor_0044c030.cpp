
class CPlayerManager { public:
    void* vftable;
    void RealDtor();
    void* CPlayerManager_scalar_deleting_destructor(unsigned int flags);
};

void* CPlayerManager::CPlayerManager_scalar_deleting_destructor(unsigned int flags)
{
    this->RealDtor();
    if (flags & 1)
        operator delete(this);
    return this;
}