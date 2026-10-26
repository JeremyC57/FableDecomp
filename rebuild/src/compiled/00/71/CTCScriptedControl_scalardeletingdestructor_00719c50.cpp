class CTCScriptedControl { public:
    void* vftable;
};

void __fastcall CTCScriptedControl_dtor(CTCScriptedControl* control);
void operator delete(void*);

void* __fastcall CTCScriptedControl_scalar_deleting_destructor(CTCScriptedControl* control, int edx_unused, unsigned int flags)
{
    CTCScriptedControl_dtor(control);
    if (flags & 1)
        operator delete(control);
    return control;
}