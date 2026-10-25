// CResourceMap_00c372c0::CResourceMap_00c372c0(COwner_00c372c0* owner, void* data) -- EH-framed constructor: owner +0, data +4, +8/+0xc zeroed, the container member at +0x10 built inline (root node pointer to its embedded node, vtable 0x012B51F0, node constructed with 0 @ 0x00C194F0; EH states 0..1), +0x40..+0x48 zeroed and the flag at +0x4c set, then `this` added to the owner's registry at +8 (virtual slot 9)
// Leaf: ??0CResourceMap_00c372c0@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C372C0, 134 bytes, __fastcall `ret 8`. Built under /EHsc (the funclet is graded
//   separately).
// Library: libpk audio (no PDB/RTTI); descriptive names. The constructor counterpart of the landed destructor
//   0x00C37130 (CResourceMap_00c37130: owner +0, resource +0xc deleted, registry at owner+8 slot 8 Remove, container
//   at +0x10 with vtable 0x012B51F0 over root 0x012B1AB0). Created by the 0x50-byte `new` at 0x00C2A4E5 with
//   (host, key). The manifest's "PropertyContainerNode_Construct" is propagated.
// Callees: the node constructor 0x00C194F0 (kind 0), the registry's Add (slot 9).
// Shape: the three zero words at +0x40 are a sub-object with an inline zeroing constructor (plain member stores
//   let VC7.1 hoist the owner load above them; 133 v 134).
struct CResourceMap_00c372c0;

struct CRegistry_00c372c0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CResourceMap_00c372c0* map);       // slot 9
};

struct COwner_00c372c0 {
    unsigned char _pad[8];
    CRegistry_00c372c0 m_registry;                      // +8
};

struct CNode_00c372c0 {
    unsigned char _body[0x28];
    CNode_00c372c0(long kind);                          // 0x00C194F0
    ~CNode_00c372c0();                                  // 0x00C19250
};

struct CRoot_00c372c0 {                                 // vtable 0x012B1AB0: the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c372c0* m_pNode;                            // +4
    CRoot_00c372c0(CNode_00c372c0* node) : m_pNode(node) { }
    ~CRoot_00c372c0() { }
};

struct CContainer_00c372c0 : CRoot_00c372c0 {           // vtable 0x012B51F0
    CNode_00c372c0 m_node;                              // +8
    CContainer_00c372c0() : CRoot_00c372c0(&m_node), m_node(0) { }
    ~CContainer_00c372c0() { }
};

struct CRange_00c372c0 {
    void* m_first;
    void* m_last;
    void* m_end;
    CRange_00c372c0() : m_first(0), m_last(0), m_end(0) { }
};

struct CResourceMap_00c372c0 {                          // 0x50 bytes
    COwner_00c372c0* m_pOwner;                          // +0
    void* m_pData;                                      // +4
    long m_8;                                           // +8
    void* m_pResource;                                  // +0xc
    CContainer_00c372c0 m_entries;                      // +0x10
    CRange_00c372c0 m_range;                            // +0x40
    bool m_enabled;                                     // +0x4c
    CResourceMap_00c372c0(COwner_00c372c0* owner, void* data);
};

CResourceMap_00c372c0::CResourceMap_00c372c0(COwner_00c372c0* owner, void* data)
    : m_pOwner(owner), m_pData(data), m_8(0), m_pResource(0), m_enabled(true)
{
    m_pOwner->m_registry.Add(this);
}