// CFixedDolly::OnInitLeaf() -- take the dolly's target thing (intelligent pointer at +0xa4), look up its interface type 0x3f component, and store that component's (or, for a kind other than 1, the fallback component's) centre into DollyVector (+0xc0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B6F0, length 127 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined
// GetInterface for type 0x3f = bit 31 of the bitset word at +0x24, tested with `jns`), the component's virtual slot 28 (+0x70, its kind),
// the CTCBase member 0x0075DB90 returning a C3DVector by hidden pointer (reads the owner at +4; manifest name
// "CTCShape_InitializeFromInterface" is a descriptive global label).
// Layout: Ego_r.pdb CFixedDolly (0xf0) puts DollyVector at +0xe4; the retail object is 0x24 smaller (DollyVector +0xc0), so `this` is
// modelled locally (the target intelligent pointer at +0xa4).
// Shape: the out-parameter GetInterface template with an UNINITIALISED out pointer (its frame word shared with the lookup key: the
// absent path reads garbage, as the landed 0x0069F480 family); the found component rides in esi. The `kind == 1 ? a : b` object of the
// centre call selects between the component (register) and a second, never-assigned component pointer read from the same shared
// frame word -- retail's `mov ecx,esi ; je ; mov ecx,[esp+8]`.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetKind();                 // slot 28 (+0x70)
    CThing* m_pThing;                       // +0x04
    C3DVector GetCentre();                  // 0x0075DB90
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CFixedDolly {
    virtual void v00();
    unsigned char           _pad04[0xa0];
    CBaseIntelligentPointer m_target;       // +0xa4
    unsigned char           _padac[0x14];
    C3DVector               DollyVector;    // +0xc0
    void OnInitLeaf();
};

void CFixedDolly::OnInitLeaf()
{
    CTCBase* pComponent;
    m_target.GetPItem()->GetInterface(&pComponent, 0x3f);
    CTCBase* pFallback;
    DollyVector = (pComponent->GetKind() == 1 ? pComponent : pFallback)->GetCentre();
}
