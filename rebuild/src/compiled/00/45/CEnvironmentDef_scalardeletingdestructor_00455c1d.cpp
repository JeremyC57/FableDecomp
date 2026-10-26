#pragma optimize("s",on)
struct CSubObj { ~CSubObj(); };
class CEnvironmentDef { public:
    char pad[0x3c];
    CSubObj sub;
    ~CEnvironmentDef();
    void * scalar_deleting_destructor(unsigned int flags);
};

void * CEnvironmentDef::scalar_deleting_destructor(unsigned int flags)
{
    this->sub.~CSubObj();
    this->~CEnvironmentDef();
    if (flags & 1)
        ::operator delete(this);
    return this;
}