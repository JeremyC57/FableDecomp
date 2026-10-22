int __fastcall DrawGetEnvironment(const unsigned char* player)
{
    return *reinterpret_cast<const int*>(player + 552);
}