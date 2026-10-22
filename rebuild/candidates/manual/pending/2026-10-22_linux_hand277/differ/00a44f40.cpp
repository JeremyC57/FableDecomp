// C3DSkeletalMorphTarget::CombineMorph(const C3DSkeletalMorphTarget&, float) -- merge another target's bone scalings into this one (both sorted by bone-name CRC): matching bones add (scaling - 1) * weight, missing bones are inserted as (1,1,1) + (scaling - 1) * weight
// Grade: DIFFER(516v525) -- esi/edi swapped (cursor vs vector), the bone-morph CRC zero store dead-eliminated, int/float stores interleaved in the insert arm
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44F40, 528 bytes (ret 8).
// Callees: CCharString::CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0, vector<C3DSkeletalBoneMorph>::insert(iterator,
//   const T&) 0x00A46B50 (catalog "Vector_Insert_CParticleMorphEntry", propagated). Constant 1.0f @ 0x0122DED8.
// Layout: retail C3DSkeletalMorphTarget is 0x10 bytes (Name +0x00, BoneMorphs +0x04 as a 12-byte STLport vector);
// C3DSkeletalBoneMorph is 0x14 bytes = PDB (BoneName +0x00, BoneNameCRC +0x04, Scaling +0x08), as in the landed
// GetBoneScaling 0x00A44CD0.
// Shape: one cursor into this target's list survives the walk over the other target; the element count is the signed
// 20-byte division recomputed at every use (unsigned compares); the miss arm builds a named bone morph (CRC zeroed by its
// constructor, then copied), assigns a (1,1,1) temporary and adds the weighted delta before the insert.
struct C3DVector
{
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};

struct CCharString
{
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    ~CCharString();                                 // 0x0099EAE0
};

struct C3DSkeletalBoneMorph                         // 0x14 bytes
{
    CCharString BoneName;                           // +0x00
    unsigned long BoneNameCRC;                      // +0x04
    C3DVector Scaling;                              // +0x08
    C3DSkeletalBoneMorph() : BoneNameCRC(0) { }
};

struct CBoneMorphArray                              // STLport vector<C3DSkeletalBoneMorph>
{
    C3DSkeletalBoneMorph* first;
    C3DSkeletalBoneMorph* last;
    C3DSkeletalBoneMorph* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    C3DSkeletalBoneMorph* begin() const { return first; }
    C3DSkeletalBoneMorph& operator[](unsigned long n) const { return *(begin() + n); }
    C3DSkeletalBoneMorph* insert(C3DSkeletalBoneMorph* position, const C3DSkeletalBoneMorph& value);   // 0x00A46B50
};

struct C3DSkeletalMorphTarget
{
    CCharString Name;                               // +0x00
    CBoneMorphArray BoneMorphs;                     // +0x04
    void CombineMorph(const C3DSkeletalMorphTarget& target, float weight);
};

void C3DSkeletalMorphTarget::CombineMorph(const C3DSkeletalMorphTarget& target, float weight)
{
    unsigned long i = 0;
    for (unsigned long t = 0; t < target.BoneMorphs.size(); ++t)
    {
        const C3DSkeletalBoneMorph& source = target.BoneMorphs[t];
        while (i < BoneMorphs.size() && BoneMorphs[i].BoneNameCRC < source.BoneNameCRC)
            ++i;
        if (i < BoneMorphs.size() && BoneMorphs[i].BoneNameCRC == source.BoneNameCRC)
        {
            BoneMorphs[i].Scaling += (source.Scaling - C3DVector(1.0f, 1.0f, 1.0f)) * weight;
        }
        else
        {
            C3DSkeletalBoneMorph morph;
            morph.BoneNameCRC = source.BoneNameCRC;
            morph.Scaling = C3DVector(1.0f, 1.0f, 1.0f);
            morph.Scaling += (source.Scaling - C3DVector(1.0f, 1.0f, 1.0f)) * weight;
            BoneMorphs.insert(BoneMorphs.begin() + i, morph);
            ++i;
        }
    }
}
