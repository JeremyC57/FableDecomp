#pragma optimize("s", on)
extern void __fastcall DestructMember_00455e59(void* member);
extern void __fastcall CSimVoicesDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CSimVoicesDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00455e59(static_cast<unsigned char*>(def) + 60);
    CSimVoicesDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}