struct CMessageOnSetThingOwner {
    void* vftable;
};

void __fastcall CMessageOnSetThingOwner_dtor(CMessageOnSetThingOwner* message);
void operator delete(void*);

void* __fastcall CMessageOnSetThingOwner_scalar_deleting_destructor(CMessageOnSetThingOwner* message, int edx_unused, unsigned int flags)
{
    CMessageOnSetThingOwner_dtor(message);
    if (flags & 1)
        operator delete(message);
    return message;
}