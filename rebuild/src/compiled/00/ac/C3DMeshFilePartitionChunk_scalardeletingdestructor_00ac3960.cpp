#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall C3DMeshFilePartitionChunk_Destruct(void* chunk);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall C3DMeshFilePartitionChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(chunk) + 16);
    C3DMeshFilePartitionChunk_Destruct(chunk);
    if (flags & 1)
        OperatorDelete(chunk);
    return chunk;
}