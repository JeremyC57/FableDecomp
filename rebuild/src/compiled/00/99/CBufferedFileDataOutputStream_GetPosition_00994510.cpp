int __fastcall CBufferedFileDataOutputStream_GetPosition(const unsigned char* stream)
{
    return *reinterpret_cast<const int*>(stream + 32);
}