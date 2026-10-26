// std::vector<CShoveHaPennyRegionDef>::resize(unsigned int newSize) -- STLport one-argument resize: resize(newSize, value_type()) with a default shove-ha'penny region def built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D26E0, length 83 bytes (+ int3 padding), `ret 4`. Caller: 0x007D2801 in
// the persist helper 0x007D2740 (manifest "CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>"). The
// frame element's vtable 0x0124383C is CShoveHaPennyRegionDef (rtti-vtables.tsv) and Ego_r.pdb CShoveHaPennyRegionDef is
// 0x30 bytes (Start +0x28, Score +0x2c) -- the 0x30-byte frame slot. The manifest label
// (_Dest_val<allocator<pair<CCharString,CScriptInfo>>>) is propagated and wrong.
// Callees: CResource::CResource @ 0x009FBEC0, resize(unsigned int, const CShoveHaPennyRegionDef&) @ 0x007D2670 (manifest
// "vector<CVectorMap<...>>::resize", propagated), CResource::~CResource @ 0x009FC550.
// Layout: the landed CDefClassBase ctor 0x0042D9E0's chain -- CResource (0x1c, out-of-line ctor/dtor) <- CDefPointeeBase
// (vtable 0x01230BA0) <- CDefClassBase (vtable 0x01230BCC: PDefManager +0x1c, GlobalIndex +0x20, three status bits +0x24)
// <- CShoveHaPennyRegionDef.
// Shape: the landed resize(n) form; the def's inline constructors fold to CResource(), PDefManager = 0, the three bits
// cleared through dl and the final vtable store (scheduled among the argument pushes); its generated destructors keep only
// CDefPointeeBase's inline user destructor's vtable store before the out-of-line ~CResource.
class CResource { public:                                           // vtable 0x012354A4, 0x1c bytes
    unsigned char _resource[0x18];                           // +0x04 refcount, resource list links, size, last used frame
    CResource();                                             // 0x009FBEC0
    virtual ~CResource();                                    // 0x009FC550
};

class CDefPointeeBase : public CResource { public:                         // vtable 0x01230BA0
    CDefPointeeBase() { }
    virtual ~CDefPointeeBase() { }
};

class CDefinitionManager;

class CDefClassBase : public CDefPointeeBase { public:                     // vtable 0x01230BCC
    CDefinitionManager* PDefManager;                         // +0x1c
    unsigned long       GlobalIndex;                         // +0x20 (left uninitialised)
    unsigned char       Setup : 1;                           // +0x24 bit 0
    unsigned char       Template : 1;                        // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;              // +0x24 bit 2
    virtual void GetInstantiationName();
    CDefClassBase() : PDefManager(0)
    {
        Setup = 0;
        Template = 0;
        DefaultValsApplied = 0;
    }
};

class CShoveHaPennyRegionDef : public CDefClassBase { public:              // vtable 0x0124383C, 0x30 bytes
    float Start;                                             // +0x28
    float Score;                                             // +0x2c
    virtual void GetInstantiationName();
};

namespace std {
template<class T> struct vector {                            // STLport vector: start/finish/end
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize, const T& value);       // 0x007D2670 for T = CShoveHaPennyRegionDef
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
}

template struct std::vector<CShoveHaPennyRegionDef>;