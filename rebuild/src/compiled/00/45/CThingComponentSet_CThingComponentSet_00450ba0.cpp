void operator delete[](void*);

class CThingComponentSet { public:
    void* buf;
    char flag;
};

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    operator delete[](self->buf);
    self->buf = 0;
    self->flag = 0;
}