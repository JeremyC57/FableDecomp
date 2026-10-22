#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CScriptModuleOpenGate_Destruct(void* gate);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScriptModuleOpenGate_CompositeScalarDeletingDestructor(void* gate, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(gate) + 84);
    CScriptModuleOpenGate_Destruct(gate);
    if (flags & 1)
        OperatorDelete(gate);
    return gate;
}