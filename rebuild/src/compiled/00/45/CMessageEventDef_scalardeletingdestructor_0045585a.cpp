#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CMessageEventDef { public: int f0; };

struct CLBP_Vtbl {
    void* CMessageEventDef_scalar_deleting_destructor(unsigned int flags);
};

void* CLBP_Vtbl::CMessageEventDef_scalar_deleting_destructor(unsigned int flags)
{
    CMessageEventDef* message = (CMessageEventDef*)this;
    ((CParentDefClassBase*)(message))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1) {
        operator delete(message);
    }
    return message;
}