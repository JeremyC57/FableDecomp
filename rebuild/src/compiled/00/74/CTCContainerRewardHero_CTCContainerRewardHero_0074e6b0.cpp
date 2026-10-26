// CTCContainerRewardHero::CTCContainerRewardHero(CThing& owner) -- the inlined CTCContainer constructor (CTCBase(owner), an empty ContainerItems vector), Enabled and CreateObjectsThatTimeOut set, then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E6B0, length 44 bytes.
// Leaf: ??0CTCContainerRewardHero
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800. Vtable 0x01269424 = CTCContainerRewardHero (rtti-vtables.tsv); the chunk
// label "CTCActivationTrigger::CTCActivationTrigger" is propagated and rejected. Ego_r.pdb: `??0CTCContainerRewardHero@@QAE
// @AAVCThing@@@Z`; CTCContainer = CTCBase + vector<pair<long,CCharString>> ContainerItems +0x0c (12 bytes in retail), then
// CTCContainerRewardHero's bool Enabled +0x18 and bool CreateObjectsThatTimeOut +0x19 (PDB +0x1c/+0x1d).
// Shape: the intermediate CTCContainer constructor is inline -- its vtable store is dead under the derived one, its vector
// member's inline constructor zeroes three words; the derived bools are plain initialisers, all ahead of the vtable store.
class CThing;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
};

struct SContainerItem;                         // std::pair<long, CCharString>

struct CContainerItemVector {                  // retail std::vector (12 bytes)
    SContainerItem* m_first;
    SContainerItem* m_last;
    SContainerItem* m_end;
    CContainerItemVector() : m_first(0), m_last(0), m_end(0) { }
};

class CTCContainer : public CTCBase { public:
    CContainerItemVector ContainerItems;       // +0x0c
    virtual void v00();
    CTCContainer(CThing& owner) : CTCBase(owner) { }
};

class CTCContainerRewardHero : public CTCContainer { public: // vtable 0x01269424
    bool Enabled;                              // +0x18
    bool CreateObjectsThatTimeOut;             // +0x19
    virtual void v00();
    CTCContainerRewardHero(CThing& owner);
};

CTCContainerRewardHero::CTCContainerRewardHero(CThing& owner)
    : CTCContainer(owner), Enabled(true), CreateObjectsThatTimeOut(true)
{
}