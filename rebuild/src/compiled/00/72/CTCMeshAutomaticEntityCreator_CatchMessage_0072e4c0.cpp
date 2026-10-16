// CTCMeshAutomaticEntityCreator::CatchMessage(const CMessageOnMeshReplaced& message) -- the owner's mesh was replaced: kill (not immediately) every live owned thing that carries interface 0x32, prune the thing-owner list, then create the child things again
// Leaf: CatchMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E4C0, length 144 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCThingOwner, type 0x21 = bit 1 of
// the byte at CThing+0x24), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80 (false),
// CTCThingOwner::ClearDeadEntries @ 0x0071B240, CTCMeshAutomaticEntityCreator::CreateChildThings @ 0x0072E290.
// Layout (retail, local model): CThing bitset +0x20 (type 0x32 = bit 18 of +0x24), map +0x44, dead bit +0x91 bit 0; CTCThingOwner
// ObjectList +0x0C (PDB; the retail STLport list is the 4-byte sentinel pointer).
// Shape: HasTC is `if (bits & mask) return true; return false;` (`test dword ptr [eax+24h],40000h`; a `!= 0` form gives `shr ; test cl,1`
// and changes the register allocation); the walk re-reads the list end each pass; VC keeps the constant 1 of the dead test in bl and spills `this` to the frame.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
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
    void Kill(bool immediately);                // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SThingNode {
    SThingNode* Next;
    SThingNode* Prev;
    CBaseIntelligentPointer Thing;
};
struct CTCThingOwner {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    SThingNode* ObjectList;                     // +0x0c list<CIntelligentPointer<CThing>> sentinel
    void ClearDeadEntries();                    // 0x0071B240
};
struct CMessageOnMeshReplaced;
struct CTCMeshAutomaticEntityCreator {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void CreateChildThings();                   // 0x0072E290
    void CatchMessage(const CMessageOnMeshReplaced& message);
};

void CTCMeshAutomaticEntityCreator::CatchMessage(const CMessageOnMeshReplaced& message)
{
    CTCThingOwner* owner;
    if (ParentThing->GetInterface(&owner, 0x21)) {
        for (SThingNode* it = owner->ObjectList->Next; it != owner->ObjectList; it = it->Next) {
            CThing* thing = it->Thing.GetPItem();
            if (thing && !(thing->m_flags91 & 1) && thing->HasTC(0x32))
                thing->Kill(false);
        }
        owner->ClearDeadEntries();
    }
    CreateChildThings();
}