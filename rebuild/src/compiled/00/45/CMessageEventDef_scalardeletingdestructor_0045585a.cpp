#pragma optimize("s",on)
struct CMessageEventDef { int f0; };
extern "C" void __fastcall CMessageEventDef_dtor(CMessageEventDef* message);
extern void __cdecl op_delete(void* p);

struct CLBP_Vtbl {
    void* CMessageEventDef_scalar_deleting_destructor(unsigned int flags);
};

void* CLBP_Vtbl::CMessageEventDef_scalar_deleting_destructor(unsigned int flags)
{
    CMessageEventDef* message = (CMessageEventDef*)this;
    CMessageEventDef_dtor(message);
    if (flags & 1) {
        op_delete(message);
    }
    return message;
}