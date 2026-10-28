// CTCUnholyPower::SetPTarget() -- clear the spell target, then take the owner's z-target (targeting component slot 33) when it is a live creature and the owner is the player creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793D20, length 220 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//          CIntelligentPointer<const CThing>::CIntelligentPointer(const CThing*) @ 0x00668B60 (vtable 0x0123E7D8),
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 8, bitset word 0 bit 8:
//          `test ch,1`), the targeting component's slot 33 (+0x84), CThing::PeekPlayer @ 0x004C7A10.
// Label: the catalog says CTCDivineWrath, but the only caller is CTCUnholyPower::FrameUpdate (0x00795300, vtable
//        0x0126BFDC slot 9) and the CTCDivineWrath copy of this body is 0x0078E820; both classes share the PDB layout.
// Shape: `PTarget = CIntelligentPointer<...>()` -- the first temporary's constructor is inlined (base ctor, vtable,
//        SetPItem(0)), the second, converting one is the out-of-line copy
//        in a named block-scoped local (a temporary reuses the ctor's eax); both temporaries share the frame word with the
//        interface lookup key. The component is the one-ternary entry select, so it lands straight in ecx for the virtual.
// Layout: retail CTCUnholyPower sits 4 bytes below the PDB here: PTarget +0x6D (PDB +0x71, an odd-offset
//         CIntelligentPointer, so the pointer classes are pack(1)), IsZTargetting +0x75 (PDB +0x79). CPlayer::ZTargeting +0x20A.
class CTCBase;
class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerConstThing() { SetPItem(0); }
    CIntelligentPointerConstThing(const CThing* item);              // 0x00668B60
    __forceinline CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
#pragma pack(pop)

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CPlayer { public:
    unsigned char _pad00[0x20a];
    bool ZTargeting;                                // +0x20A
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C (bit 3 creature, bit 5 player creature)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};

struct CTCTargeting {                               // interface type 8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetZTarget();                   // slot 33 (+0x84)
};

class CTCUnholyPower { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x6d - 0x08];
    CIntelligentPointerConstThing PTarget;          // +0x6D CIntelligentPointer<CThingCreatureBase>
    bool IsZTargetting;                             // +0x75
    void SetPTarget();
};

void CTCUnholyPower::SetPTarget()
{
    PTarget = CIntelligentPointerConstThing();
    IsZTargetting = false;
    if (ParentThing->HasTC(8)) {
        CThing* target = ((CTCTargeting*)ParentThing->GetTC(8))->GetZTarget();
        if (target && !(target->m_flags91 & 1) && (target->ThingTypeFlags & 8)
            && (ParentThing->ThingTypeFlags & 0x20)) {
            {
                CIntelligentPointerConstThing newTarget(target);
                PTarget = newTarget;
            }
            IsZTargetting = ParentThing->PeekPlayer().ZTargeting;
        }
    }
}