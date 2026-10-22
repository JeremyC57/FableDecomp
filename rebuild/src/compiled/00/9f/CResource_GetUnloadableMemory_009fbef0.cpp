int __fastcall CResource_GetUnloadableMemory(const unsigned char* resource)
{
    return *reinterpret_cast<const int*>(resource + 20);
}