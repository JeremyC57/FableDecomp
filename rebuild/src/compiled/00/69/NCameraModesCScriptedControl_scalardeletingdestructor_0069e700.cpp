#pragma optimize("t", on)
extern void __fastcall DestructMember_005ed2c0(void* member);
extern void __fastcall CScriptedControl_Destruct(void* control);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScriptedControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    DestructMember_005ed2c0(static_cast<unsigned char*>(control) + 164);
    CScriptedControl_Destruct(control);
    if (flags & 1)
        OperatorDelete(control);
    return control;
}