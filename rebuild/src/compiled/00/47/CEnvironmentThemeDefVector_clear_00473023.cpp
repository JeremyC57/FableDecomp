#pragma optimize("s", on)
struct AutoTinyActiveFile
{
    void* resource;
    void* reader;
};
extern void __fastcall AutoTinyFinishRead_00450b1b(void* resource, void* reader, char* status);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall OnReadFinished(AutoTinyActiveFile* self)
{
    char status;
    AutoTinyFinishRead_00450b1b(self->resource, self->reader, &status);
    if (self->resource != 0)
        free(self->resource);
}