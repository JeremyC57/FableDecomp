class CTCOpinionSource { public:
    void* vftable;
};

void __fastcall CTCOpinionSource_dtor(CTCOpinionSource* source);
void operator delete(void*);

void* __fastcall CTCOpinionSource_scalar_deleting_destructor(CTCOpinionSource* source, int edx_unused, unsigned int flags)
{
    CTCOpinionSource_dtor(source);
    if (flags & 1)
        operator delete(source);
    return source;
}