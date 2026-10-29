// CThingAICreature::SetNewBrain(CDefPointer<const CAIBrainDef> brainDef) -- tells the idle scheduler (when present) a new brain is coming, builds a brain for this creature from the def over the display engine, world, thing and player managers, and installs it in the counted brain pointer
// Grade: RELOCATION_MATCH (retry helper C) -- the thing-manager argument is read through an inline accessor GetThingManager() (an inlined call is evaluated before the real getter calls), so g_pThingManager loads into ebx ahead of GetPlayerManager and the brain takes ebp, as retail. Was DIFFER(176v178), parked as a whole-program mod/ref gap.
// Leaf: SetNewBrain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833010, 178 bytes (+14 int3). `ret 4` (the def pointer passed by value).
// Callees: CVectorMap::LowerBound(const long*) 0x0040F020, the idle scheduler's brain-reset 0x008018B0 (manifest
//          "LTextBinTree_RebuildWalkerArray" propagated; named by use), operator new 0x00BFEA1A (0xA8 bytes), CThing::
//          GetPlayerManager() 0x004C7A00 (manifest "CViewBase::GetDisplayEngine" propagated), CWorld* CThing::PeekWorld()
//          0x004C7AA0, CDisplayEngine* CThing::GetDisplayEngine() 0x004C7A70, CAIBrain::CAIBrain(...) 0x0088C160,
//          CCountedPointer<CAIBrain>::Reset(CAIBrain*) 0x00834E40 on PBrain, CDefClassBase virtual slot 1 (Delete).
// Shape: the 0xD3 presence bit is word +0x38 bit 19 (the lookup through the map pointer in edi); the `new` expression's null
//        test skips the constructor; the by-value def copy for the constructor is built on the stack (`push ecx ; mov [esp],edi`)
//        with its add-ref, the manager getters evaluated after it; the parameter's own copy is released at the end.
// Layout: CThing presence bits +0x20, Components +0x44; PBrain +0x1A8 (Ego_r.pdb +0x1D8, 0x30 lower in retail).
class CTCBase;
class CTCIdleScheduler { public:                            // interface type 0xD3
    void OnNewBrain_008018b0();                      // 0x008018B0 (manifest label propagated)
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
struct CAIBrainDef;
struct CDefPointer {                                 // CDefPointer<const CAIBrainDef>
    const CDefClassBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++const_cast<CDefClassBase*>(Object)->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (--const_cast<CDefClassBase*>(Object)->RefCount == 0)
                const_cast<CDefClassBase*>(Object)->Delete();
        }
    }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CDisplayEngine; class CWorld; class CThingManager; class CPlayerManager; class CThingAICreature;
extern CThingManager* g_pThingManager;               // 0x013B8A1C
inline CThingManager* GetThingManager() { return g_pThingManager; }
class CAIBrain { public:                                    // 0xA8 bytes
    unsigned char _body[0xa8];
    CAIBrain(CDisplayEngine* display, CWorld* world, CThingManager* things, CPlayerManager* players, CThingAICreature* owner, CDefPointer def);   // 0x0088C160
};
struct CBrainPointer {                               // CCountedPointer<CAIBrain>
    void Reset(CAIBrain* brain);                     // 0x00834E40
};
class CThingAICreature { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x1a8 - 0x4c];
    CBrainPointer PBrain;                            // +0x1a8
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70
    CWorld* PeekWorld();                             // 0x004C7AA0
    void SetNewBrain(CDefPointer brainDef);
};
void CThingAICreature::SetNewBrain(CDefPointer brainDef)
{
    CTCIdleScheduler* scheduler;
    if (GetInterface(&scheduler, 0xd3))
        scheduler->OnNewBrain_008018b0();
    CAIBrain* brain = new CAIBrain(GetDisplayEngine(), PeekWorld(), GetThingManager(), GetPlayerManager(), this, brainDef);
    PBrain.Reset(brain);
}