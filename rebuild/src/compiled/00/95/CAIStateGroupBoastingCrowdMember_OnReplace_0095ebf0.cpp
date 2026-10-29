// CAIStateGroup_BoastingCrowdMember::OnReplace() -- when the boaster is alive and has a crowd component (interface 0x74), tells that component this creature has left the crowd
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasInterface + the ternary GetTC model on a GetPItem() thing local: the crowd loads straight into ecx before the creature argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095EBF0, length 107 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on the boaster pointer at +0x14),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x74 = bitset
// word 3 bit 20 -> `test dword ptr [eax+2Ch],100000h`), the component's virtual slot 29 (+0x74) taking PBrain->Creature.
// Shape: the same guard chain as IsStillValid (0x0095E870) as nested ifs falling to the shared epilogue; the
// component goes straight from the map entry into ecx and the creature argument is loaded after it.
// Model: local (no engine header for this class).
class CTCBase;
class CAIBrain;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* FindInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CTCBoastingCrowd {                       // interface 0x74
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void RemoveCrowdMember(CThing* creature);  // slot 29 (+0x74)
};

class CAIBrain;
struct CTCBoastingCrowd;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBoaster;           // +0x14
    void OnReplace();
};

void CAIStateGroup_BoastingCrowdMember::OnReplace()
{
    if (PBoaster.GetPItem() != 0 && !PBoaster.GetPItem()->IsDead()) {
        CThing* boaster = PBoaster.GetPItem();
        if (boaster->HasInterface(0x74)) {
            CTCBoastingCrowd* crowd = boaster->GetTC<CTCBoastingCrowd>(0x74);
            crowd->RemoveCrowdMember(PBrain->Creature);
        }
    }
}