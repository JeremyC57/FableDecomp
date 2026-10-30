// CGameScriptInterface::OpinionSourceSetAsExclusive(const CScriptThing& thing, bool exclusive) const -- when the script thing holds a thing (slot 11, no validity check) with the opinion-source interface (type 0x50: bitset word 2 bit 16): make it the opinion reaction manager's exclusive source (CWorld+0x74, 0x00700770) when asked, else clear the exclusive source when it is this thing (the intelligent pointer at +0x714, GetPItem @ 0x00A01B50)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894E60, length 89 bytes.
// Callees: the script thing's virtual slot 11 (+0x2c) GetThing (no validity check), the inline HasInterface bitset test, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the manager's exclusive-source setter @ 0x00700770 (label propagated). Shape: the interface in edi, the thing in esi, the manager re-read through World in every arm, `ret 8` tail-duplicated.
// Leaf: OpinionSourceSetAsExclusive
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
class CWorld;
class CCharString;
class CScriptThing;
struct CDefPointer_Retail;
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020 (a second copy of the same body sits at 0x004365B0)
};
struct CThingPhysics_Retail;
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual CThing_Retail* GetStandingOnThing();    // slot 38 (+0x98)
    unsigned char         _pad04[0xc];              // +0x04 .. +0x10
    unsigned char         m_type;                   // +0x10 (1 = the player creature, 2 = an AI creature, 3 = a building)
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44 the sorted {type, component} map
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    unsigned char         _pad64[8];
    unsigned char         m_flags;                  // +0x6c (bit 1 = physical, bit 3 = creature)
    unsigned char         _pad6d[0x24];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned char         m_92_0 : 8;               // +0x92
    unsigned char         m_93_0 : 1;               // +0x93
    unsigned char         m_killedByScript : 1;     //       bit 1
    unsigned char         m_93_2 : 6;
    void Kill(bool flag);                           // 0x004C9B80 (CThing::Kill)
    CScriptThing GetScriptThing();                  // 0x004C7E50 (CThing::GetScriptThing, by value through the hidden result pointer)
    void AssignHomeBuilding(CThing_Retail* building);   // 0x00512210 (landed as AssignTarget_00512210; the label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated)
    void SetNewBrain(CDefPointer_Retail brain);     // 0x00833010 (CThingAICreature::SetNewBrain, the counted def pointer by value)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }   // the masked word (a bool would narrow the test)
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
struct CThingManager_Retail {
    unsigned char _pad[0x55];
    bool          m_allEntitiesPaused;              // +0x55
    void SetNonScriptedEntitesPaused(bool paused);  // 0x0051EFC0
};
struct CBulletTimeManager_Retail {
    void SetBulletTimeSlowdownMultiplier(long priority, unsigned long key, float multiplier);   // 0x004C6170
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140 (the label "CTCInventoryExperience::ProcessButtonBReleased" is propagated)
};
struct CMessageEvent_Retail;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:   // the bound filter holds POINTERS to its operands (landed 0x00939D70)
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
    void RemoveMessage(const CMessageEvent_Retail* message);   // 0x00493B5B
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
struct CTCOpinionSource_Retail;
struct COpinionReactionManager_Retail {
    unsigned char                  _pad[0x714];
    CBaseIntelligentPointer m_exclusiveSource;   // +0x714 (a CIntelligentPointer)
    void* GetExclusiveOpinionSource() { return m_exclusiveSource.GetPItem(); }   // inline: the intelligent pointer's item
    const CCharString& GetThingAttitudeToPlayerAsString(CThing_Retail* thing);   // 0x007018B0
    float GetOpinionValue(CThing_Retail* thing, long opinion);   // 0x00701750 (labelled COpinionReactionManager_GetOpinionValue)
    bool AttachOpinionSource(CThing_Retail* thing, const CCharString& name, CTCOpinionSource_Retail** ppSource);   // 0x00700780
    bool AttachOpinionSource(CThing_Retail* thing, long id, CTCOpinionSource_Retail** ppSource);   // 0x00700820
    void ClearOpinionSource(CThing_Retail* thing, bool flag);   // 0x007008C0
    void SetExclusiveOpinionSource(CThing_Retail* thing);   // 0x00700770 (16 bytes before AttachOpinionSource; the manifest label is propagated)
};
class CWorld { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CThingManager_Retail* GetThingManager();   // slot 3 (+0x0c)
    unsigned char                   _pad04[0x5c];               // +0x04 .. +0x60
    CMessageEventManager_Retail*    PMessageEventManager_Data;  // +0x60 (engine/CWorld.h)
    unsigned char                   _pad64[4];
    CBulletTimeManager_Retail*      PBulletTimeManager_ptr;     // +0x68
    unsigned char                   _pad6c[8];
    COpinionReactionManager_Retail* POpinionReactionManager_Data;   // +0x74
    unsigned char                   _pad78[0x68];
    bool                            CreatureGenerationEnabled;  // +0xe0
    COpinionReactionManager_Retail* GetOpinionReactionManager() { return POpinionReactionManager_Data; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::OpinionSourceSetAsExclusive(const CScriptThing& thing, bool exclusive) const
{
    CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
    if (t && t->HasInterface(0x50)) {
        if (exclusive)
            World->POpinionReactionManager_Data->SetExclusiveOpinionSource(t);
        else if (World->POpinionReactionManager_Data->GetExclusiveOpinionSource() == t)
            World->POpinionReactionManager_Data->SetExclusiveOpinionSource(0);
    }
}