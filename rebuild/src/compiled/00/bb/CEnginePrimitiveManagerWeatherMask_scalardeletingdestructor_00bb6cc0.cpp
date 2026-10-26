class CEnginePrimitiveManagerWeatherMask { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerWeatherMask_dtor(CEnginePrimitiveManagerWeatherMask* mask);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerWeatherMask_scalar_deleting_destructor(CEnginePrimitiveManagerWeatherMask* mask, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerWeatherMask_dtor(mask);
    if (flags & 1)
        operator delete(mask);
    return mask;
}