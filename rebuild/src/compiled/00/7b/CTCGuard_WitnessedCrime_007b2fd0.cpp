// CTCGuard::WitnessedCrime(const COpinionDeed& deed, long deedHandle) -- a village guard saw a crime: unless its village already handled the deed, note whether the guard itself is the perpetrator, record the crime's severity (from the hero's deed log effects) and the frame, report it to the village and, for a severity 2 or 3 crime, have the village recalculate its security sweep
// Leaf: WitnessedCrime
// SW2 sweep (advisor/007b2fd0/probe/x1.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2FD0, 336 bytes (`ret 8`). Ego_r.pdb
// `?WitnessedCrime@CTCGuard@@QAEXABVCOpinionDeed@@J@Z`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interfaces 0x23 village member, 0x51 hero deed log),
// CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::HasCrimeBeenReported(long) @ 0x0065914B (descriptive), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50, CWorld::PeekMyPlayer @ 0x0049D9D0, CPlayer::GetPControlledCreature @ 0x00487DC0, CTCHeroOpinionDeedLog::
// GetDeedEffectsDef(long) @ 0x006C5630 (CDefPointer by hidden pointer), CWorld::GetFrame @ 0x0049D870, CTCGuard's crime report
// @ 0x007B2C70 (village, handle, deed type, self flag; named descriptively), CTCVillage::RecalculateSecuritySweep(long) @ 0x00659C32.
// Globals: the main game component @ 0x013B86A0 (+0x24 the world).
// Layout: CTCGuard retail = Ego_r.pdb + 0x10 past the deque (STLport deque 0x28 bytes): LastCrimeSeenSeverity +0x44,
// FrameLastCrimeSeen +0x48. COpinionDeed: Type +0x00, the perpetrator's intelligent pointer +0x18. COpinionDeedEffectsDef
// severity +0x70.
// Shape: the deed log comes through the landed GetInterface idiom (uninitialised pointer, the dead argument slot read when the
// interface is absent); the effects def pointer's release is the DecRef / GetRefCount form (as the landed InformOfDeed
// 0x00683590); the self test reads ParentThing before the GetPItem call.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CWorld { public:
    CPlayer* PeekMyPlayer() const;                      // 0x0049D9D0
public:
    static long __fastcall GetFrame();   // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld* World;                                      // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0x70 - 0x08];
    long Severity;                                      // +0x70 ECrimeSeverity
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
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
class COpinionDeed { public:
    long Type;                                          // +0x00 EOpinionDeedType
    unsigned char _pad04[0x14];
    CIntelligentPointer<CThing> Perpetrator;                // +0x18
};
class CTCHeroOpinionDeedLog { public:                          // interface 0x51
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};
class CTCVillage { public:
    void RecalculateSecuritySweep(long flags);          // 0x00659C32
public:
    bool HasCrimeBeenReported(long);   // 0x0065914B
};
class CTCVillageMember { public:                               // interface 0x23
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};

class CTCGuard { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x44 - 0x08];
    long          LastCrimeSeenSeverity;                // +0x44
    long          FrameLastCrimeSeen;                   // +0x48
    void ReportCrime(CTCVillage* village, long deedHandle, long deedType, bool self);   // 0x007B2C70
    void WitnessedCrime(const COpinionDeed& deed, long deedHandle);
};

void CTCGuard::WitnessedCrime(const COpinionDeed& deed, long deedHandle)
{
    if (!ParentThing->HasTC(0x23))
        return;
    CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetPVillage();
    if (!village)
        return;
    if (village->HasCrimeBeenReported(deedHandle))
        return;
    CThing* parent = ParentThing;
    bool self = deed.Perpetrator.GetPItem() == parent;
    CThing* hero = g_pMainGameComponent->World->PeekMyPlayer()->GetPControlledCreature();
    bool severe = false;
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed.Type);
        LastCrimeSeenSeverity = effects->Severity;
        if (LastCrimeSeenSeverity == 2)
            severe = true;
        else if (LastCrimeSeenSeverity == 3)
            severe = true;
        FrameLastCrimeSeen = CWorld::GetFrame();
    }
    ReportCrime(village, deedHandle, deed.Type, self);
    if (severe)
        village->RecalculateSecuritySweep(0);
}