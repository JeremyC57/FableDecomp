int __fastcall DrawGetEnvironment(const unsigned char* event)
{
    return *reinterpret_cast<const int*>(event + 40);
}