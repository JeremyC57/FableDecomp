// CParticleProgram::AddGroup(const C3DGroup2& group) -- add a mesh group to the program: append an empty group to both the simulated and the non-simulated group arrays, give both the source group's bone, then re-index every vertex blend of the source through the particle and vertex remap tables (entries of -1 are dropped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE37D0, length 332 bytes (+ int3 padding), `ret 4`, 8-aligned frame.
// Identity: Ego_r.pdb ?AddGroup@CParticleProgram@@QAEXABVC3DGroup2@@@Z.
// Callees: C3DGroup2::C3DGroup2() 0x00AC40C0, CArray<C3DGroup2>::push_back 0x00AE83D0, free 0x00BFEA14 (the temporary's
// blend array), C3DGroup2::SetBoneIndex 0x00AC4070, C3DGroup2::AddVertexBlend 0x00AC4110 (manifest labels propagated).
// Layout: Ego_r.pdb CParticleProgram with the retail CArray 12 bytes (PDB 16): ParticleIndices +0xDC, VertexIndices
// +0xE8, SimGroups +0xFC, NonSimGroups +0x108; C3DGroup2 (16 bytes retail): BoneIndex +0, VertexBlends +4;
// C3DVertexBlend2 {VertexIndex, BlendValue}.
// Shape: the empty group pushed is an argument temporary (its blend storage freed inline); both back() references
// are taken before the bone stores; the source blend is read through a reference into two named locals (the dead
// `lea` of the element stays) and each re-indexed blend is a by-value temporary in the 8-aligned frame.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class C3DVertexBlend2 { public:
    unsigned long VertexIndex;
    float         BlendValue;
    C3DVertexBlend2() {}
    C3DVertexBlend2(unsigned long index, float value) : VertexIndex(index), BlendValue(value) {}
};
template <class T> class CArray { public:                      // retail 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return _M_start[n]; }
    T& back() { return *(_M_finish - 1); }
    void push_back(const T& x);
};
class C3DGroup2 { public:
    long                    BoneIndex;                  // +0
    CArray<C3DVertexBlend2> VertexBlends;               // +4
    C3DGroup2();                                        // 0x00AC40C0
    ~C3DGroup2() { if (VertexBlends._M_start) free(VertexBlends._M_start); }
    void SetBoneIndex(long bone);                       // 0x00AC4070
    void AddVertexBlend(const C3DVertexBlend2& blend);  // 0x00AC4110
};
template <> void CArray<C3DGroup2>::push_back(const C3DGroup2& x);   // 0x00AE83D0
class CParticleProgram { public:
    unsigned char     _pad000[0xdc];
    CArray<long>      ParticleIndices;                  // +0xdc
    CArray<long>      VertexIndices;                    // +0xe8
    bool              BezierEnable;                     // +0xf4
    float             AveragePatchSize;                 // +0xf8
    CArray<C3DGroup2> SimGroups;                        // +0xfc
    CArray<C3DGroup2> NonSimGroups;                     // +0x108
    void AddGroup(const C3DGroup2& group);
};
void CParticleProgram::AddGroup(const C3DGroup2& group)
{
    SimGroups.push_back(C3DGroup2());
    NonSimGroups.push_back(C3DGroup2());
    C3DGroup2& sim = SimGroups.back();
    C3DGroup2& nonSim = NonSimGroups.back();
    sim.SetBoneIndex(group.BoneIndex);
    nonSim.SetBoneIndex(group.BoneIndex);
    for (unsigned long i = 0; i < group.VertexBlends.size(); ++i) {
        const C3DVertexBlend2& blend = group.VertexBlends[i];
        unsigned long vertex = blend.VertexIndex;
        float value = blend.BlendValue;
        if (vertex < ParticleIndices.size() && ParticleIndices[vertex] != -1)
            sim.AddVertexBlend(C3DVertexBlend2(ParticleIndices[vertex], value));
        if (vertex < VertexIndices.size() && VertexIndices[vertex] != -1)
            nonSim.AddVertexBlend(C3DVertexBlend2(VertexIndices[vertex], value));
    }
}