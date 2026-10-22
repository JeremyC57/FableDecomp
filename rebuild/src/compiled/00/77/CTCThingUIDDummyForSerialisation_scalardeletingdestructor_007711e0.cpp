struct CTCThingUIDDummyForSerialisation {
    void* vftable;
};

void __fastcall CTCThingUIDDummyForSerialisation_dtor(CTCThingUIDDummyForSerialisation* serialisation);
void operator delete(void*);

void* __fastcall CTCThingUIDDummyForSerialisation_scalar_deleting_destructor(CTCThingUIDDummyForSerialisation* serialisation, int edx_unused, unsigned int flags)
{
    CTCThingUIDDummyForSerialisation_dtor(serialisation);
    if (flags & 1)
        operator delete(serialisation);
    return serialisation;
}