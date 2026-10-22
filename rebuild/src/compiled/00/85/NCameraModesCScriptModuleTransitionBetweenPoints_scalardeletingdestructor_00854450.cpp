#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CScriptModuleTransitionBetweenPoints_Destruct(void* points);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScriptModuleTransitionBetweenPoints_CompositeScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(points) + 76);
    CScriptModuleTransitionBetweenPoints_Destruct(points);
    if (flags & 1)
        OperatorDelete(points);
    return points;
}