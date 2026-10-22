int __fastcall CResource_GetDebugMemoryInUse(const unsigned char* resource)
{
    return *reinterpret_cast<const int*>(resource + 20);
}