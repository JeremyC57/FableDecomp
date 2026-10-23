#pragma optimize("t", on)
extern void __fastcall DestructMember_0099eae0(void* member);
extern void __fastcall CConsoleCommandLine_Destruct(void* line);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CConsoleCommandLine_CompositeScalarDeletingDestructor(void* line, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(line) + 4);
    CConsoleCommandLine_Destruct(line);
    if (flags & 1)
        OperatorDelete(line);
    return line;
}