struct CScriptModuleCameraOnAStick {
    void* vftable;
};

void __fastcall CScriptModuleCameraOnAStick_dtor(CScriptModuleCameraOnAStick* stick);
void operator delete(void*);

void* __fastcall CScriptModuleCameraOnAStick_scalar_deleting_destructor(CScriptModuleCameraOnAStick* stick, int edx_unused, unsigned int flags)
{
    CScriptModuleCameraOnAStick_dtor(stick);
    if (flags & 1)
        operator delete(stick);
    return stick;
}