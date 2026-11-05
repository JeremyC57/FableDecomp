// Leaf: ?CPhysicsFacesSegmentLeaf_Sizeof@@YIHPAUS@@@Z
struct S { char pad[4]; short* begin; char gap[4]; short* end; };
int __fastcall CPhysicsFacesSegmentLeaf_Sizeof(S* leaf)
{
    int n = (int)(leaf->end - leaf->begin);
    return n * 2 + 12;
}