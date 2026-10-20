// Grade: DIFFER(292v294) -- address formation only: retail tests BodyParts[part].empty() with folded [eax+ebx]/[eax+ebx+4] loads before the later add eax,ebx; ours forms the element address first (2 bytes shorter). Tried !empty(), begin()!=end(), part!=3 (fixed jl->jne)
// CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const -- pick a random texture-morph variant, then for each of the three body-part slots pick a random mesh, gather its texture morphs plus the slot's own, and add the mesh/texture set to the morph set (the first slot optionally skipped)
// Leaf: GetRandomBodyParts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086BE40, length 294 bytes (`ret 0Ch`). FableWin.pdb
// ?GetRandomBodyParts@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@_N@Z.
// Callees: GFROR13(unsigned long) @ 0x00497890 (`__fastcall`), CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long&,
// unsigned long, const CVectorMap<unsigned long, CTextureMorph>&, CArray<CMorphSetTextureMorph>&) const @ 0x0086B810 (twice),
// CArray<CMeshTextureSet>::push_back @ 0x0086E7C0, free @ 0x00BFEA14 (the set's CArray destructor).
// Layout: Ego_r.pdb with the retail 12-byte CArray and 16-byte CVectorMap: BodyParts (CArray<CArray<CBodyPartMesh> >) +0x00,
// BodyPartTextureMorphs (CArray<CVectorMap<...> >) +0x0c, MaxTextureGroupID +0x38; CBodyPartMesh (24 bytes): MeshIndex +0,
// Priority +4, TextureMorphs +8; CMorphSet::CMeshTextureSet: MeshIndex +0, TextureMorphs (CArray) +4.
// Shape: the random step is an inline helper (seed = GFROR13(seed * 0x24A1 + 0x24DF), stored before and after the call, then
// a zero-guarded modulo); the variant is that +1 under the group-count test; the per-slot set is a loop-local record
// (its CArray zeroed before the empty test, freed at the end of every pass); the slot index steps by 12 in ebx and the
// texture-map offset by 16 in the dead `seed` argument slot.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
long __fastcall GFROR13(unsigned long value);        // 0x00497890

template<class T> struct CArray {                    // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) { }
    ~CArray() { if (m_begin) free(m_begin); }
    unsigned long size() const { return m_end - m_begin; }
    bool empty() const { return m_begin == m_end; }
    T& operator[](unsigned long n) const { return *(m_begin + n); }
};

template<class K, class V> struct CVectorMap {      // 16 bytes: vector, comparator, dirty flag
    void* m_begin; void* m_end; void* m_capacity;
    unsigned char Compare;
    bool Dirty;
};

class CRandomAppearanceMorph {
public:
    struct CTextureMorph { unsigned long NewTexture; unsigned long GroupID; };
    struct CBodyPartMesh {                           // 24 bytes
        long MeshIndex;                              // +0x00
        long Priority;                               // +0x04
        CVectorMap<unsigned long, CTextureMorph> TextureMorphs;   // +0x08
    };
    struct CMorphSet {
        struct CMorphSetTextureMorph { long OriginalTexture; long NewTexture; };
        struct CMeshTextureSet {
            long MeshIndex;                          // +0x00
            CArray<CMorphSetTextureMorph> TextureMorphs;          // +0x04
        };
        struct CMeshTextureSetArray : CArray<CMeshTextureSet> {
            void push_back(const CMeshTextureSet& set);           // 0x0086E7C0
        };
        CMeshTextureSetArray BodyParts;              // +0x00
    };
protected:
    void GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const;
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long variant, const CVectorMap<unsigned long, CTextureMorph>& morphs,
                                CArray<CMorphSet::CMorphSetTextureMorph>& out) const;      // 0x0086B810
    static unsigned long Random(unsigned long& seed, unsigned long range)
    {
        seed = seed * 0x24A1 + 0x24DF;
        seed = GFROR13(seed);
        return range ? seed % range : 0;
    }
    CArray<CArray<CBodyPartMesh> >                      BodyParts;               // +0x00
    CArray<CVectorMap<unsigned long, CTextureMorph> >   BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, unsigned long>                     SkeletalMorphs;          // +0x18
    CVectorMap<unsigned long, CTextureMorph>            GeneralTextureMorphs;    // +0x28
    unsigned long                                       MaxTextureGroupID;       // +0x38
};

void CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const
{
    unsigned long variant = 0;
    if (MaxTextureGroupID)
        variant = Random(seed, MaxTextureGroupID) + 1;
    for (long part = 0; part != 3; ++part) {
        CMorphSet::CMeshTextureSet set;
        if (!BodyParts[part].empty()) {
            const CBodyPartMesh& mesh = BodyParts[part][Random(seed, BodyParts[part].size())];
            set.MeshIndex = mesh.MeshIndex;
            GetRandomTextureMorphs(seed, variant, mesh.TextureMorphs, set.TextureMorphs);
            GetRandomTextureMorphs(seed, variant, BodyPartTextureMorphs[part], set.TextureMorphs);
            if (!skipFirstPart || part != 0)
                morphSet.BodyParts.push_back(set);
        }
    }
}
