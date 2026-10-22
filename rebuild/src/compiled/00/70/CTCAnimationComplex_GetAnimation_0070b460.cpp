int __fastcall DrawGetEnvironment(const unsigned char* complex)
{
    return *reinterpret_cast<const int*>(complex + 12);
}