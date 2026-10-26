class CWideString { public: void* p; void ctor_0099ec30(const void* src); };
// Leaf: GetBankHandle
class CMemoryFile { public: char pad[0x20]; char member[4]; };
CWideString* __fastcall CMemoryFile_GetPathname(const CMemoryFile* self, void* edx, CWideString* ret)
{
    ret->ctor_0099ec30((const char*)self + 0x20);
    return ret;
}