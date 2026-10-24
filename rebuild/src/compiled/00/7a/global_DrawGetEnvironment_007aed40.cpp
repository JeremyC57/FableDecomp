int __fastcall DrawGetEnvironment(const unsigned char* self)
// Leaf: DrawGetEnvironment
{
    return *reinterpret_cast<const int*>(self + 20);
}