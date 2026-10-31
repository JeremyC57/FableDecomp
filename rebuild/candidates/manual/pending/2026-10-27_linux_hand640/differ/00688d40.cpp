// CInputProcessZTarget::AddToInterface(CInputProcessManager& manager) -- latch the player's controlled creature into PCreature; when it is alive, cache its hero combat def and, if it has a targeting component (interface 8) whose current target is alive, latch that target into PTarget; reset DeactivateHoldTimer and chain to the base AddToInterface
// Grade: DIFFER(362v360) -- register colouring tie: this in edi and the def temporary's pointer in ebp (retail this=ebp, def=edi; the two `[ebp]` derefs cost the 2 bytes); everything else (shrink-wrapped def register, &PCreature in esi, both CDefPointer releases) now matches | this session: shared engine/CDefPointer.h (this=ebp but &PCreature=edi, 358: its Assign reads other.Object once, retail twice), local CDefPointer re-reading other.Object (this file), named player local (360v360, wrong order), PCreature-> (same)
// Leaf: AddToInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688D40, 360 bytes, `ret 4`. Ego_r.pdb ?AddToInterface@CInputProcessZTarget@@UAE_NAAVCInputProcessManager@@@Z.
// Callees: CPlayer::PeekPControlledCreature() const 0x00487DD0 (bound 19x), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem
//   0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (temporaries' vtables 0x01237F14 CIntelligentPointer<const
//   CThingPlayerCreature>, 0x01237F1C CIntelligentPointer<const CThing>), CThingCreatureBase::GetPHeroCombatDef() const
//   0x00663340 (Ego_r ?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ), the component
//   map's LowerBound 0x004365B0, the targeting component's slot 33 (+0x84, current target), CInputProcessGameBase::AddToInterface
//   0x00687A70.
// Layout: Ego_r.pdb CInputProcessZTarget (0x40): CInputProcessGameBase (MyPlayer +0x18), PCreature
//   (CIntelligentPointer<const CThingPlayerCreature>) +0x28, PHeroCombatDef (CDefPointer<const CHeroCombatDef>) +0x30,
//   DeactivateHoldTimer +0x34, PTarget (CIntelligentPointer<const CThing>) +0x38. No CInputProcessZTarget header; the
//   CInputProcessGameBase header flattens the vptr. CDefPointer is local: engine/CDefPointer.h's Assign takes the pointee
//   by value, while this site re-reads other.Object after the release (header debt).
class CInputProcessManager;
struct CTCBase {
    virtual void v00();
};
struct CThing;
struct CTCTargeting {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetCurrentTarget();            // slot 33 (+0x84)
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long          m_kind;
    CTCTargeting* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x004365B0
    __forceinline CTCTargeting* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointeeBase.h"
template <class T> class CDefPointer {
public:
    __forceinline ~CDefPointer() { Release(); }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
    __forceinline void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
};
class CHeroCombatDef;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCTargeting* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CThingCreatureBase : CThing {
    CDefPointer<const CHeroCombatDef> GetPHeroCombatDef() const;   // 0x00663340
};
struct CThingPlayerCreature : CThingCreatureBase { };
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* PData;                                   // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(const CThing* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // <const CThingPlayerCreature> vtable 0x01237F14, <const CThing> 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointer(T* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { const CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    T* operator->() const { return static_cast<T*>(GetPItem()); }
};
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual bool AddToInterface(CInputProcessManager& manager);   // 0x00687A70
    unsigned char           _pad04[0x14];          // +0x04 CAInputProcess, World
    CPlayer*                MyPlayer;              // +0x18
    unsigned char           _pad1c[0x0c];          // +0x1c DefinitionManager, Component, PGamePlayerInterface
};
class CInputProcessZTarget : public CInputProcessGameBase {
public:
    CIntelligentPointer<const CThingPlayerCreature> PCreature;                    // +0x28
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;               // +0x30
    long             DeactivateHoldTimer;          // +0x34
    CIntelligentPointer<const CThing> PTarget;                      // +0x38
    virtual bool AddToInterface(CInputProcessManager& manager);
};

bool CInputProcessZTarget::AddToInterface(CInputProcessManager& manager)
{
    PCreature = CIntelligentPointer<const CThingPlayerCreature>(MyPlayer->PeekPControlledCreature());
    if (PCreature.GetPItem() && !PCreature.GetPItem()->m_dead) {
        PHeroCombatDef = static_cast<const CThingPlayerCreature*>(PCreature.GetPItem())->GetPHeroCombatDef();
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(8)) {
            CThing* target = creature->GetTC(8)->GetCurrentTarget();
            if (target && !target->m_dead)
                PTarget = CIntelligentPointer<const CThing>(target);
        }
    }
    DeactivateHoldTimer = -1;
    return CInputProcessGameBase::AddToInterface(manager);
}
