#pragma optimize("s",on)
class COpinionSourceDef { public:
    void dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* COpinionSourceDef::scalar_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}