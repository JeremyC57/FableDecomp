class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCThingUIDDummyForSerialisation { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCThingUIDDummyForSerialisation_scalar_deleting_destructor(CTCThingUIDDummyForSerialisation* serialisation, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(serialisation))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(serialisation);
    return serialisation;
}