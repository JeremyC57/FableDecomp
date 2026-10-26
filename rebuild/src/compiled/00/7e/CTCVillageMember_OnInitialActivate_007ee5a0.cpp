// CTCVillageMember::OnInitialActivate() -- re-attaches the owner to the village recorded by UID when it is not already attached, and (with the village-member check enabled) looks for a village when an owner that needs one has none recorded
// Leaf: ?OnInitialActivate@CTCVillageMember
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE5A0, 182 bytes (plain `ret`); CTCVillageMember vtable 0x0124007C slot 6
// (CTCBase::OnInitialActivate). Ego_r.pdb `?OnInitialActivate@CTCVillageMember@@UAEXXZ`.
// Callees: CTCBase::GetThingManager() 0x00686AE0; CThingManager::GetThingWithUID(unsigned __int64) 0x0051ED80; CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, interface 0x22 = word 1 bit 2);
//          CTCVillageMember::GetPVillage() const 0x007EE540 (Ego_r.pdb name; most landed callers spell it as the global
//          `__fastcall` GFGetPVillageInterface); CTCVillage::AddThingToVillage(CThing&) 0x0065B854 (Ego_r.pdb signature);
//          CTCBase::GetThingSearchTools() 0x00686D80; CThingSearchTools::GetPFirstThing<CThingFilter_HasTC, CThingVillage>(
//          const CThingFilter_HasTC&, CThingVillage**) const 0x007EE660 (its result unused: the release build keeps only the
//          search of a debug check). Global: the village-member check flag 0x013B86D8.
// Layout: CTCVillageMember (Ego_r.pdb, retail equal): ParentThing +0x04, PDef +0x0c, PVillage +0x10, VillageUID +0x18
//         (unsigned __int64); CVillageMemberDef NeedsVillageToFunctionCorrectly +0x25.
// Shape: the village comes from the unchecked out-parameter GetInterface (the absent arm reads the lookup key's frame
//        word); the 64-bit UID is tested with `or` of its halves (twice, the second re-read); the search filter is a
//        one-word temporary holding the interface type, passed with the out pointer's address.
class CTCBase;
class CTCVillage;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
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
class CThingVillage : public CThing { public: };

class CTCVillage { public:
    void AddThingToVillage(CThing& thing);      // 0x0065B854
};
class CThingManager { public:
    CThing* GetThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};
class CThingFilter_HasTC { public:
    long Type;                                  // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) {}
};
class CThingSearchTools { public:
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;   // 0x007EE660
};
extern bool g_bCheckVillageMembers_013b86d8;    // 0x013B86D8

class CVillageMemberDef { public:
    unsigned char _pad_0x00[0x25];
    bool          NeedsVillageToFunctionCorrectly;   // +0x25
};

class CTCBase
{
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
    CThingSearchTools& GetThingSearchTools();   // 0x00686D80
};

class CTCVillageMember : public CTCBase { public:
    void*              __vftable;               // +0x00
    CThing*            ParentThing;             // +0x04 (CTCBase)
    unsigned char      _pad_0x08[0x4];
    CVillageMemberDef* PDef;                    // +0x0c CDefPointer<const CVillageMemberDef>::Object
    unsigned char      PVillage[0x8];           // +0x10 CIntelligentPointer<CThing>
    unsigned __int64   VillageUID;              // +0x18
    CTCVillage* GetPVillage() const;            // 0x007EE540
    void OnInitialActivate();
};

void CTCVillageMember::OnInitialActivate()
{
    if (VillageUID != 0) {
        CThing* villageThing = GetThingManager().GetThingWithUID(VillageUID);
        if (villageThing) {
            CTCVillage* village;
            villageThing->GetInterface(&village, 0x22);
            if (village != GetPVillage())
                village->AddThingToVillage(*ParentThing);
        }
    }
    if (g_bCheckVillageMembers_013b86d8 && VillageUID == 0 && PDef && PDef->NeedsVillageToFunctionCorrectly) {
        CThingVillage* village;
        GetThingSearchTools().GetPFirstThing(CThingFilter_HasTC(0x22), &village);
    }
}