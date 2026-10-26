#pragma optimize("s",on)
extern "C" void operator_delete_stub(void*);
class COpinionSourceDef { public:
    void dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* COpinionSourceDef::scalar_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator_delete_stub(this);
    return this;
}