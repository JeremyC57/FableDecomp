struct CEngineInternalPrimitive2DText {
    void* vftable;
};

void __fastcall CEngineInternalPrimitive2DText_dtor(CEngineInternalPrimitive2DText* text);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitive2DText_scalar_deleting_destructor(CEngineInternalPrimitive2DText* text, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitive2DText_dtor(text);
    if (flags & 1)
        operator delete(text);
    return text;
}