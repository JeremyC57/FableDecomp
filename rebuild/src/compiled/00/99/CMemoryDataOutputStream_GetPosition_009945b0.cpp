int __fastcall CMemoryDataOutputStream_GetPosition(const unsigned char* stream)
{
    return *reinterpret_cast<const int*>(stream + 20);
}