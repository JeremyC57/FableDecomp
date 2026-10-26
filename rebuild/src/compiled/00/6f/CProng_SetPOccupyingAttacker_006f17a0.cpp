// CProng::SetPOccupyingAttacker(CThingCreatureBase* attacker) -- make the attacker the prong's occupier (released first, then set)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F17A0, length 127 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10 (temporary vtable 0x0123E7D8 = CIntelligentPointer<CThingCreatureBase>).
// Layout: Ego_r.pdb CProng: POccupier +0x14.
// Shape: two copies of the landed SetPTarget temporary-pointer shape (0x007A7150): a null item, then the attacker.
// Shape: the helper names the temporary's GetPItem() CALL result (`occupier = pointer.GetPItem(); POccupier.SetPItem(occupier);`),
//   which keeps `add esi,14h` (this of SetPItem) after the GetPItem call as retail; the inline expression form hoists it.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem();                                // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x0123E7D8 (CIntelligentPointer<CThingCreatureBase>)
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CThingCreatureBase { public:                                 // only the dead bit is read here
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                               // +0x91 bit 0
    unsigned char m_91_1 : 7;
};

extern long g_framesPerSecond;                              // 0x01375550

struct CProng {
    bool                    Blocked;                        // +0x00
    unsigned char           Prong[0xc];                     // +0x01 C3DVector (packed in the PDB)
    unsigned char           _pad0d[3];
    long                    ProngBlockageTimer;             // +0x10
    CBaseIntelligentPointer POccupier;                      // +0x14 CIntelligentPointer<CThingCreatureBase>
    __forceinline void SetPOccupierItem(CBaseObject* item)  // the landed SetPTarget shape: a temporary pointer set to the
    {                                                       // item, read back into the member, destroyed
        CIntelligentPointer pointer;
        pointer.SetPItem(item);
        CBaseObject* occupier = pointer.GetPItem();
        POccupier.SetPItem(occupier);
    }
    CThingCreatureBase* GetPOccupier() { return (CThingCreatureBase*)POccupier.GetPItem(); }
    void SetBlocked(bool blocked);
    void SetPOccupyingAttacker(CThingCreatureBase* attacker);
};

void CProng::SetPOccupyingAttacker(CThingCreatureBase* attacker)
{
    SetPOccupierItem(0);
    SetPOccupierItem((CBaseObject*)attacker);
}