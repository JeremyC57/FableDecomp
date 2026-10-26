class CScriptedMapBrush { public:
    void* vftable;
};

void __fastcall CScriptedMapBrush_dtor(CScriptedMapBrush* brush);
void operator delete(void*);

void* __fastcall CScriptedMapBrush_scalar_deleting_destructor(CScriptedMapBrush* brush, int edx_unused, unsigned int flags)
{
    CScriptedMapBrush_dtor(brush);
    if (flags & 1)
        operator delete(brush);
    return brush;
}