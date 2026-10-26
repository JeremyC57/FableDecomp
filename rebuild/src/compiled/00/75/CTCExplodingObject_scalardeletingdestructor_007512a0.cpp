class CTCExplodingObject { public:
    void* vftable;
};

void __fastcall CTCExplodingObject_dtor(CTCExplodingObject* explodingObject);
void operator delete(void*);

void* __fastcall CTCExplodingObject_scalar_deleting_destructor(CTCExplodingObject* explodingObject, int edx_unused, unsigned int flags)
{
    CTCExplodingObject_dtor(explodingObject);
    if (flags & 1)
        operator delete(explodingObject);
    return explodingObject;
}