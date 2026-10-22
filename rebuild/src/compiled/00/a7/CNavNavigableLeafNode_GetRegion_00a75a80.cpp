int __fastcall CNavNavigableLeafNode_GetRegion(const unsigned char* node)
{
    return *reinterpret_cast<const int*>(node + 16);
}