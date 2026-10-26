class CWideString { public: void* p; void ctor(const void* src); };
class CMemoryFile { public: char pad[0x20]; char member[4]; };
CWideString* __fastcall CMemoryFile_GetPathname(const CMemoryFile* file, void* edx, CWideString* ret)
{
    ret->ctor((const char*)file + 0x20);
    return ret;
}