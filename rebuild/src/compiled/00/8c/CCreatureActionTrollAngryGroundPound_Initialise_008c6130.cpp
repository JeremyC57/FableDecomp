// CCreatureAction_TrollAngryGroundPound::Initialise() -- calls this action's virtual slot 81 (the pound set-up), runs the base Initialise, then clears bit 6 of the creature's byte at +0x1c8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6130, length 48 bytes.
// Callees: this object's virtual slot 81 (`call [eax+144h]`, no arguments), CCombatActionBase::Initialise @ 0x00857520 (non-virtual call),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8).
// Shape: the virtual call goes through `this` (a plain dynamic dispatch outside any constructor), then the read-modify-write `and byte ptr [..],0BFh` on a named creature local (the same `&=` on the bare call expression loads, re-bases the address with `add eax,1C8h` and stores separately; a one-bit bitfield behaves the same).
class CThingCreatureBase { public:                 // only the flags byte at +0x1c8 is modelled
    unsigned char _pad[0x1c8];
    unsigned char m_flags1c8;               // +0x1c8 bit 6: pounding (set by PerformAction @ 0x008C63A0, landed)
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

class CCombatActionBase { public:                  // retail sizeof 0x114; slots 0..80 of the retail vtable precede the derived virtual
    virtual void v00();
    virtual void Initialise();              // slot 1: 0x00857520
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_TrollAngryGroundPound : public CCombatActionBase { public:
    virtual void Initialise();
    virtual void SetUpPound();              // slot 81
};

void CCreatureAction_TrollAngryGroundPound::Initialise()
{
    SetUpPound();
    CCombatActionBase::Initialise();
    CThingCreatureBase* pCreature = PCreature.GetPItem();   // named local: a read-modify-write on an inline call expression re-bases the address
    pCreature->m_flags1c8 &= 0xbf;
}