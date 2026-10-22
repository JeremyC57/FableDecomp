int __fastcall DrawGetEnvironment(const unsigned char* boss)
{
    return *reinterpret_cast<const int*>(boss + 24);
}