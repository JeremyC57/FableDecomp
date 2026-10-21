// Grade: DIFFER(362v360) -- this/def registers swap (ours this=edi, def=ebp; retail this=ebp, def=edi); without __forceinline VC7.1 keeps the PTarget temporary's ctor/op= out of line (332v360)
// CInputProcessZTarget::AddToInterface(CInputProcessManager& manager) -- latch the player's controlled creature into PCreature; when it is alive, cache its hero combat def and, if it has a targeting component (interface 8) whose current target is alive, latch that target into PTarget; reset DeactivateHoldTimer and chain to the base AddToInterface
// Leaf: AddToInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688D40, length 360 bytes, `ret 4`.
// Callees: CPlayer::GetControlledCreature @ 0x00487DD0 (`add ecx,2Ch ; jmp GetPItem`), CBaseIntelligentPointer ctor
//   @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (temporaries' vtables 0x01237F14
//   CIntelligentPointer<const CThingPlayerCreature> and 0x01237F1C CIntelligentPointer<const CThing>),
//   CThingPlayerCreature::GetHeroCombatDef() @ 0x00663340 (CDefPointer by hidden pointer; manifest "CEventKilledCreature"
//   is propagated), the component map's LowerBound @ 0x004365B0, the targeting component's virtual slot 33 (+0x84,
//   current target), CInputProcessGameBase::AddToInterface @ 0x00687A70.
// Layout: Ego_r.pdb CInputProcessZTarget (0x40 bytes): CInputProcessGameBase (MyPlayer +0x18) then PCreature +0x28,
//   PHeroCombatDef +0x30, DeactivateHoldTimer +0x34, PTarget +0x38 -- modelled locally because the class has no engine
//   header and the base's own virtual is called. CThing dead flag bit 0 of +0x91, component bitset +0x20 (8 = `test ch,1`),
//   map +0x44. CDefPointer as the landed CDefinitionManager::GetDef 0x0083BD10 (short release in operator=, long one in
//   the temporary's dtor).
// Shape: the creature is re-read through PCreature.GetPItem() for the null test, the dead test and the def call, then once
//   into a named local for the HasTC/GetTC pair.
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
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CHeroCombatDef : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer {
    CHeroCombatDef* m_p;
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCTargeting* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CThingPlayerCreature : CThing {
    CDefPointer GetHeroCombatDef() const;          // 0x00663340
};
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* PData;                                   // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // <const CThingPlayerCreature> vtable 0x01237F14, <const CThing> 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointer(T* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    T* operator->() const { return static_cast<T*>(GetPItem()); }
};
class CPlayer {
public:
    CThing* GetControlledCreature();               // 0x00487DD0
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
    CIntelligentPointer<CThingPlayerCreature> PCreature;                    // +0x28
    CDefPointer      PHeroCombatDef;               // +0x30
    long             DeactivateHoldTimer;          // +0x34
    CIntelligentPointer<CThing> PTarget;                      // +0x38
    virtual bool AddToInterface(CInputProcessManager& manager);
};

bool CInputProcessZTarget::AddToInterface(CInputProcessManager& manager)
{
    PCreature = CIntelligentPointer<CThingPlayerCreature>(static_cast<CThingPlayerCreature*>(MyPlayer->GetControlledCreature()));
    if (PCreature.GetPItem() && !PCreature.GetPItem()->m_dead) {
        PHeroCombatDef = static_cast<CThingPlayerCreature*>(PCreature.GetPItem())->GetHeroCombatDef();
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(8)) {
            CThing* target = creature->GetTC(8)->GetCurrentTarget();
            if (target && !target->m_dead)
                PTarget = CIntelligentPointer<CThing>(target);
        }
    }
    DeactivateHoldTimer = -1;
    return CInputProcessGameBase::AddToInterface(manager);
}
