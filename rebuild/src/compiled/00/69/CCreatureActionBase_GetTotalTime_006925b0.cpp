int __fastcall DrawGetEnvironment(const unsigned char* action)
{
    return *reinterpret_cast<const int*>(action + 16);
}