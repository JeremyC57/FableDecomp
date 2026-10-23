#pragma optimize("t", on)
extern void __fastcall DestructMember_00b97af0(void* member);
extern void __fastcall CEngineSceneMap_Destruct(void* map);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineSceneMap_CompositeScalarDeletingDestructor(void* map, int, unsigned int flags)
{
    DestructMember_00b97af0(static_cast<unsigned char*>(map) + 24);
    CEngineSceneMap_Destruct(map);
    if (flags & 1)
        OperatorDelete(map);
    return map;
}