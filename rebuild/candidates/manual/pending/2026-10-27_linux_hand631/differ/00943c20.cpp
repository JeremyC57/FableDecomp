// CAIStateGroup_WorkInWarehouse::IsRunnable() -- a villager in working hours whose village offers it a crate-carrying task (remembered in PTask) is runnable
// Grade: DIFFER(206v206) -- register colouring only: retail tests the villager bit through eax (`mov eax,[edi+4] ; mov eax,[eax+18h] ; test byte ptr [eax+24h],8`), then forms PBrain/creature/&key in ecx/eax/edx; ours uses ecx for the first test and edx/eax/eax for the second block
// Leaf: ?IsRunnable@CAIStateGroup_WorkInWarehouse@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00943C20, 206 bytes, virtual, plain `ret`.
// Callees: CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours() const 0x0091A6F0 (Ego_r.pdb IBE), CVectorMap::LowerBound
//   0x0040F020 (inlined out-parameter GetInterface: interface 0x23 = bit 3 of +0x24; the miss path reads the never-written slot),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (PDB QBE; landed callers spell it GFGetPVillageInterface), CTCVillage::
//   GetBestCrateTaskForThing(const CThing&) 0x00658EA0 (landed), CIntelligentPointer<CVillageTaskCarryCrate> temporary
//   (vtable 0x0128B0B0: CBaseIntelligentPointer ctor 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, dtor 0x00A01C10).
// Names (chunk 631): Ego_r.pdb CAIStateGroup_WorkInWarehouse has one member, `CIntelligentPointer<CVillageTaskCarryCrate> PTask`
//   at +0x1c (the earlier PTaskThing/CThing spelling was a guess), and the task comes from GetBestCrateTaskForThing (a const
//   CThing& argument); IsInWorkHours is the protected const member of the CAIStateGroup_WorkBase<CAIStateGroupBase> template.
// Tried: the PDB respell (this file), a GetCreature() accessor (one or all three reads), a named creature local, const
//   GetInterface, an early `return false` (209), a `!= 0` HasTC (211). Earlier: masked-word HasTC, bool-return HasTC.
// Next: lane gap: scratch-register colouring of the first block.
class CTCBase;
class CThing;
class CTCVillage;
class CVillageTaskCarryCrate;
enum ETCInterfaceType { };
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CInterfaceMap {
public:
    struct SEntry { ETCInterfaceType first; CTCBase* second; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                         // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};
class CTCVillage {
public:
    CVillageTaskCarryCrate* GetBestCrateTaskForThing(const CThing& thing);   // 0x00658EA0
};
template <class TBase> class CAIStateGroup_WorkBase : public TBase {
protected:
    bool IsInWorkHours() const;                 // 0x0091A6F0
};
class CAIStateGroup_WorkInWarehouse : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
public:
    unsigned char _pad_0x14[8];
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;   // +0x1c
    virtual bool IsRunnable();
};

bool CAIStateGroup_WorkInWarehouse::IsRunnable()
{
    if (PBrain->PCreature->HasTC(0x23) && IsInWorkHours()) {
        CTCVillageMember* villager;
        PBrain->PCreature->GetInterface(&villager, 0x23);
        CTCVillage* village = villager->GetPVillage();
        if (village != 0) {
            PTask = village->GetBestCrateTaskForThing(*PBrain->PCreature);
            if (PTask.GetPItem() != 0)
                return true;
        }
    }
    return false;
}
