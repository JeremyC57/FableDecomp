// CTCBattleCharge::SetPTarget(CThing* target, bool isZTargetting) -- point PTarget at the target through a temporary CIntelligentPointer, then record IsZTargetting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A7150, length 81 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (on
// the temporary, then on PTarget), GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv): the inlined derived constructor's store.
// Identity: Ego_r.pdb CTCBattleCharge: PTarget +0xc, IsZTargetting PDB +0x8c = retail +0x88 (one STL container 4 bytes narrower in retail). Called from 0x008CD620. The manifest label "CTCAssassinRush::SetPTarget" is propagated.
// Shape: the landed AssignTarget family (0x007C76D0): an 8-byte stack CIntelligentPointer built by
// the inline default constructor, pointed at the argument, read back into the member pointer and
// destroyed; the byte argument is stored after the temporary's destructor, so the temporary lives in
// an inner scope and the flag assignment follows it.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCBattleCharge { public:
    unsigned char           _pad00[0x0c];                   // vptr, CTCBase
    CBaseIntelligentPointer PTarget;                        // +0x0c
    unsigned char           _pad14[0x88 - 0x14];
    bool                    IsZTargetting;                  // +0x88
    void SetPTarget(CThing* target, bool isZTargetting);
};

void CTCBattleCharge::SetPTarget(CThing* target, bool isZTargetting)
{
    {
        CIntelligentPointer pointer;
        pointer.SetPItem((CBaseObject*)target);
        PTarget.SetPItem(pointer.GetPItem());
    }
    IsZTargetting = isZTargetting;
}