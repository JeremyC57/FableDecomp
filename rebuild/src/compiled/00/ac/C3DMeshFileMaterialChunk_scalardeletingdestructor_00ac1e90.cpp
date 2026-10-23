#pragma optimize("t", on)
extern void __fastcall DestructMember_0099eae0(void* member);
extern void __fastcall C3DMeshFileMaterialChunk_Destruct(void* chunk);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall C3DMeshFileMaterialChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(chunk) + 76);
    C3DMeshFileMaterialChunk_Destruct(chunk);
    if (flags & 1)
        OperatorDelete(chunk);
    return chunk;
}