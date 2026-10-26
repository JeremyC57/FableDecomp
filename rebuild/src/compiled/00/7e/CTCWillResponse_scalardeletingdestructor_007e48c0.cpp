class CTCWillResponse { public:
    void* vftable;
};

void __fastcall CTCWillResponse_dtor(CTCWillResponse* response);
void operator delete(void*);

void* __fastcall CTCWillResponse_scalar_deleting_destructor(CTCWillResponse* response, int edx_unused, unsigned int flags)
{
    CTCWillResponse_dtor(response);
    if (flags & 1)
        operator delete(response);
    return response;
}