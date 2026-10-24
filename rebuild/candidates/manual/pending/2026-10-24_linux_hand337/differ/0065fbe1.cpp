// Grade: DIFFER(76v74) -- the building pointer address is formed as `add eax,168h ; mov ecx,eax` (retail `lea ecx,[eax+168h]`), same for +0x170
// HasBuildingAsHomeOrWork::operator()(const CIntelligentPointer<CThingAICreature>& creature) -- true when the creature is alive and its home building (+0x168), else its work building (+0x170), is the held building
// Leaf: ??RHasBuildingAsHomeOrWork
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065FBE1, 74 bytes (`ret 4`, bool in al). Size-optimised TU (odd address,
// the CTCVillage security-sweep TU): `#pragma optimize("s", on)` (this body keeps no frame pointer).
// Callees: IS_THING_ALIVE<CIntelligentPointer<CThingAICreature> > 0x004AB8F0 (out of line, `__fastcall`),
// CBaseIntelligentPointer::GetPItem 0x00A01B50 (four times: creature, home; creature, work).
// Layout: Ego_r.pdb HasBuildingAsHomeOrWork {CThing* Building} (4 bytes); CThingAICreature HomeBuilding / WorkBuilding at
// retail +0x168 / +0x170 (Ego_r.pdb +0x190 / +0x198), as the landed 0x007BB660 filter model.
// Shape: the creature pointer is re-read through GetPItem for each building; home first, then work (the
// CThingFilter_HasBuildingAsHomeOrWork filter of 0x007BB660 tests work first); `mov al,1 ; jmp` / `xor al,al` tail.
#pragma optimize("s", on)
struct CThing;
struct CBaseIntelligentPointer {
    void*   VTable;
    CThing* PItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThing {
    unsigned char _pad_0x00[0x168];
};
// CIntelligentPointer<CThingBuilding>, read straight through CBaseIntelligentPointer::GetPItem (a typed inline
// forwarder adds the member offset with `add eax ; mov ecx,eax` instead of retail's `lea ecx,[eax+168h]`).
struct CBuildingPointer : CBaseIntelligentPointer { };
struct CThingAICreature : CThing {
    CBuildingPointer HomeBuilding;                    // +0x168
    CBuildingPointer WorkBuilding;                    // +0x170
};

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0

struct HasBuildingAsHomeOrWork {
    CThing* Building;                                 // +0x00
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;
};

bool HasBuildingAsHomeOrWork::operator()(const CIntelligentPointer<CThingAICreature>& creature) const
{
    if (IS_THING_ALIVE(creature)) {
        if (creature.GetPItem()->HomeBuilding.GetPItem() == Building ||
            creature.GetPItem()->WorkBuilding.GetPItem() == Building)
            return true;
    }
    return false;
}
