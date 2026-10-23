#pragma optimize("s", on)
struct AutoTinyActiveFile
{
    void* resource;
    void* reader;
};
extern void __fastcall AutoTinyFinishRead_00cd3625(void* resource, void* reader, char* status);
extern void __cdecl AutoTinyReleaseResource(void* resource);
void __fastcall OnReadFinished(AutoTinyActiveFile* self)
{
    char status;
    AutoTinyFinishRead_00cd3625(self->resource, self->reader, &status);
    if (self->resource != 0)
        AutoTinyReleaseResource(self->resource);
}