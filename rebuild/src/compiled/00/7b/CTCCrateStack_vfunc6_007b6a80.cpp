// CTCCrateStack::vfunc_6() -- initial activation: unless the game subsystem says otherwise, take the crate def index from the owning village's def; a stack with no village or no crate def kills its thing
//
// Retail 0x007B6A80, 137 bytes (plain `ret`). Vtable 0x0126DA14 (CTCCrateStack) slot 6.
// Callees: the CTCBase accessor @ 0x00686D60 (`[0x013B8A1C]+0x1c`, unidentified subsystem; its virtual
// slot 9 returns a bool -- landed CTCAmbientBird::OnInitialActivate 0x007E1F70 model),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CTCVillageMember::GetPVillage
// @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" propagated), the village def's virtual slot 1 (release
// at zero), CThing::Kill(bool) @ 0x004C9B80.
// Layout = Ego_r.pdb CTCCrateStack: CrateDefIndex +0x1c. The village's def pointer is at +0x94 in
// retail (the PDB's PDef_Object is +0x120 behind its wider containers), the def's crate index at +0x40.
// Shape: the out-parameter GetInterface (interface 0x23, `test byte [eax+24h],8`) with an
// uninitialised out pointer (stale `mov eax,[esp+4]`); `CDefPointer<const CVillageDef> def =
// village->PDef;` is a local copy (null-tested add-ref) whose destructor releases WITHOUT a null test
// (`dec [ecx+4] ; jne`); CrateDefIndex is re-read for the test; one shared Kill(false).
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
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
    void Kill(bool removeNow);                              // 0x004C9B80
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_refCount;                                        // +4
};

class CVillageDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x40 - 0x08];
    long          CrateDefIndex;                            // +0x40
};

template <class T>
class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (--Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CTCVillage_Model {
    unsigned char           _pad00[0x94];
    CDefPointer<CVillageDef> PDef;                          // +0x94 CDefPointer<const CVillageDef>
};

class CTCVillageMember { public:
    CTCVillage_Model* GetPVillage() const;                  // 0x007EE540
};

struct CGameSubsystem {                                     // the main game component's member at +0x1c (unidentified)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressed();                            // slot 9 (+0x24)
};

class CTCCrateStack { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];                      // +0x08 CTCBase, FirstCrate, SecondCrate
    long          CrateDefIndex;                            // +0x1c
    CGameSubsystem* GetGameSubsystem();                     // 0x00686D60 (CTCBase)
    void vfunc_6();
};

void CTCCrateStack::vfunc_6()
{
    if (!GetGameSubsystem()->IsSuppressed())
    {
        CTCVillageMember* member;
        ParentThing->GetInterface(&member, 0x23);           // TC_VILLAGE_MEMBER
        CTCVillage_Model* village = member->GetPVillage();
        if (village)
        {
            {
                CDefPointer<CVillageDef> def = village->PDef;
                CrateDefIndex = def->CrateDefIndex;
            }
            if (CrateDefIndex != 0)
                return;
        }
        ParentThing->Kill(false);
    }
}