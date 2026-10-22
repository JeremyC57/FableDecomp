int __fastcall CCachedFile_GetPosition(const unsigned char* file)
{
    return *reinterpret_cast<const int*>(file + 4);
}