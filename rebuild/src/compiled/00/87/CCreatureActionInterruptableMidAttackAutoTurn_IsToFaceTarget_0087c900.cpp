// CCreatureAction_InterruptableMidAttackAutoTurn::IsToFaceTarget() const -- whether the creature's player has the auto-face flag (+0x20a) set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C900, length 27 bytes. Vtable slot 73 of CCreatureAction_InterruptableMidAttackAutoTurn (0x012778CC).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (`add ecx,8`: PCreature at +8, this is never kept), CThing::GetPlayer
// @ 0x004C7A10 (the player record for the thing's player index byte +0x90).
// Shape: frameless; the byte is normalised with `test cl,cl ; setne al` and no `xor eax,eax` -- the `? true : false` bool
// form on an unsigned char member (GOTCHAS). The player's +0x20a byte has no PDB name (CPlayer's layout is padding there).
class CPlayer { public:                            // engine/CPlayer.h exists but is not used: +0x20a is padding there
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;         // +0x20a (name descriptive)
};

class CThing
{
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};
class CThingPhysical : public CThing
{
public:
};
class CThingGameObject : public CThingPhysical
{
public:
};

class CThingCreatureBase_Calls : public CThingGameObject { public:           // engine/CThingCreatureBase.h exists but is not used: only this call is modelled
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

class CCreatureAction_InterruptableMidAttackAutoTurn { public:   // engine/CCreatureAction_InterruptableMidAttackAutoTurn.h exists but is not used: retail CCreatureActionBase is 0xa8 bytes, the PDB build's 0xb4
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase_Calls> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    bool IsToFaceTarget() const;
};

bool CCreatureAction_InterruptableMidAttackAutoTurn::IsToFaceTarget() const
{
    return PCreature.GetPItem()->PeekPlayer().m_autoFaceTarget ? true : false;
}