class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
};
struct CMessageOnSetThingOwner {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CMessageOnSetThingOwner_scalar_deleting_destructor(CMessageOnSetThingOwner* message, int edx_unused, unsigned int flags)
{
    ((CBaseClass*)(message))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(message);
    return message;
}