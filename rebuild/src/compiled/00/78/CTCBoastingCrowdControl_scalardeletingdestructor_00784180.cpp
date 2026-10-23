#pragma optimize("t", on)
extern void __fastcall DestructMember_00783ff0(void* member);
extern void __fastcall CTCBoastingCrowdControl_Destruct(void* control);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCBoastingCrowdControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    DestructMember_00783ff0(static_cast<unsigned char*>(control) + 16);
    CTCBoastingCrowdControl_Destruct(control);
    if (flags & 1)
        OperatorDelete(control);
    return control;
}