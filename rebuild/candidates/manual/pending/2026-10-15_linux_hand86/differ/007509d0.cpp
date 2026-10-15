// CTCDiscovery::SetDiscovered(bool discovered, const CThing* discoverer) -- store the flag and the discoverer (dropped when not discovered)
// Grade: DIFFER(ours v retail) -- 90v90, register swap only: retail keeps `this` in esi and the discoverer in edi, the lane the reverse (tried: a named local copy of the discoverer, a named CIntelligentPointer local)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007509D0, length 90 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (x2),
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01237F1C = CIntelligentPointer.
// PDB CTCDiscovery (0x18 bytes): Discovered +0x0c, Discoverer (CIntelligentPointer<const CThing>) +0x0d
// (really unaligned: `lea ecx,[esi+0Dh]`; modelled under pack(1)).
// Shape: `ret 8`; the discoverer is cleared only when it is non-null and the flag is false (`jne ; test edi,edi ;
// je ; xor edi,edi`: the explicit `&& discoverer != 0` test is kept); the flag store is scheduled after the
// temporary's `lea`; the pointer goes through an inline CIntelligentPointer(item) temporary and the inline
// operator= (SetPItem(other.GetPItem())), the temporary dies through the out-of-line base destructor.
struct CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCDiscovery {
    virtual void v00();
    unsigned char       _base[0x8];                         // CTCBase: ParentThing +4, flags +8/+9
    bool                Discovered;                         // +0x0c
    CIntelligentPointer Discoverer;                         // +0x0d CIntelligentPointer<const CThing>
    void SetDiscovered(bool discovered, CBaseObject* discoverer);
};
#pragma pack(pop)

void CTCDiscovery::SetDiscovered(bool discovered, CBaseObject* discoverer)
{
    if (!discovered && discoverer != 0)
        discoverer = 0;
    Discovered = discovered;
    Discoverer = CIntelligentPointer(discoverer);
}
