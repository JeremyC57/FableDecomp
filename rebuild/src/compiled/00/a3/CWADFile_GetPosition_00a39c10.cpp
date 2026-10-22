int __fastcall CWADFile_GetPosition(const unsigned char* file)
{
    return *reinterpret_cast<const int*>(file + 12);
}