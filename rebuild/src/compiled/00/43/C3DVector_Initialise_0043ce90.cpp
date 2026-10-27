// C3DVector::Initialise(float x, float y, float z) -- set the three components
// Leaf: ?Initialise@C3DVector@@QAEXMMM@Z
// Retail: Fable.exe @ 0x0043CE90, 23 bytes, __fastcall `ret 0Ch`, no return value: the three arguments copied as dwords
// (`mov eax,[esp+4] ; mov edx,[esp+8] ; mov [ecx],eax ; mov eax,[esp+0Ch] ; mov [ecx+4],edx ; mov [ecx+8],eax`).
// Identity (probable): an unreferenced out-of-line copy of a header inline -- the same 23 bytes stand at ~400 other
// retail addresses (0x0040D970, 0x004476A0, 0x0050C190, ... none called), i.e. a three-member setter that nearly every
// TU instantiates. Of the PDB name sets' void three-argument setters (QAEXMMM / QAEXJJJ), C3DVector::Initialise is the
// one of a ubiquitous three-member class (C3DCoordI::Initialise(long, long, long) would compile to the same bytes; the
// float vector is by far the more common header inline). The float arguments are copied without x87 traffic.
class C3DVector {
public:
    float X, Y, Z;
    void Initialise(float x, float y, float z);
};

void C3DVector::Initialise(float x, float y, float z)
{
    X = x;
    Y = y;
    Z = z;
}