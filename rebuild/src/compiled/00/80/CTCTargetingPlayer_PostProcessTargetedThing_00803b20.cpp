// CTCTargetingPlayer::PostProcessTargetedThing() -- refreshes the threatening state when the targeted thing is no longer the last threatened one, then reports the parent thing's controlled-creature check
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00803B20, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 on PLastThreatenedThing, the virtual GetPTargetedThing()
// (slot 33, +0x84), CTCTargetingPlayer::UpdateTargetThreatening() @ 0x00803280, and the parent thing's bool check
// @ 0x0085EE10 (type byte +0x10 == 2 plus a script-thing query; manifest "CTCScriptedControl::IsActive" is a propagated
// name, so it is named descriptively here) as the tail jump.
// Layout: Ego_r.pdb CTCTargetingPlayer PLastThreatenedThing is at +0xdc, but the two 16-byte PDB vectors at +0x68/+0x78 are
// 12 bytes in retail, so the member sits at +0xd4 (consistent with the landed slot-36 getter 0x004E2328 reading
// PTargetLockedThing at +0xe4 = PDB +0xec). engine/CTCTargetingPlayer.h therefore disagrees after +0x68 and the class is
// a local model.
// Shape: the old pointer is read first into a named local (edi) because retail fetches it before the virtual call; the
// final call's result is returned (sibling jump through `mov ecx,[esi+4]`).
class CBaseObject;

class CThing { public:
    bool IsControlledCreature();             // 0x0085EE10
};

struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* m_pItem;                    // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

class CTCTargetingPlayer { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();     // slot 33 (+0x84)
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[0xcc];           // CTCTargetingBase + player members up to the PDB vectors' retail size
    CIntelligentPointer<CThing> PLastThreatenedThing;   // +0xd4
    void UpdateTargetThreatening();          // 0x00803280
    bool PostProcessTargetedThing();
};

bool CTCTargetingPlayer::PostProcessTargetedThing()
{
    CThing* lastThreatened = PLastThreatenedThing.Get();
    if (GetPTargetedThing() != lastThreatened)
        UpdateTargetThreatening();
    return ParentThing->IsControlledCreature();
}