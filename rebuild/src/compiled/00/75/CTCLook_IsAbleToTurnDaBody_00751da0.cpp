// CTCLook::IsAbleToTurnDaBody() const -- the body may turn unless the parent creature exists, is not Dead (CThing bit at +0x91), is performing an action and that action's owner (+0x100) says the body is locked (virtual slot 50)
// Grade: RELOCATION_MATCH (retry wave 2 sec2b: the alive test through the inline IS_THING_ALIVE<CThingCreatureBase*>(const T&) template reads PParentCreature into eax and copies it to ecx like retail; was 57v59)
// Leaf: ?IsAbleToTurnDaBody@CTCLook@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751DA0, length 59 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0 (bool), the virtual slot 50 (+0xc8, bool) of the object
// at creature +0x100 (modelled as an opaque action-controller interface).
// Layout (local model, no engine header): PParentCreature at +0x10 (PDB offset).
// Shape: every test falls through to the shared `mov al,1` tail; only the final virtual result returns `xor al,al`; the
// creature pointer is first read into a local (eax, copied to ecx for the call) and re-read from the MEMBER after the
// call (a member read in the first test loads straight into ecx).
struct CActionController {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual bool IsBodyTurnLocked();               // slot 50 (+0xc8)
};

class CThingCreatureBase { public:
    unsigned char      _pad_0x00[0x91];
    unsigned char      m_thingFlags;             // +0x91: CThing bitfield byte, bit 0 = Dead (PDB +0xa1)
    unsigned char      _pad_0x92[0x100 - 0x92];
    CActionController* m_pActionController;      // +0x100
    bool IsDead() const { return (m_thingFlags & 1) != 0; }
    bool IsPerformingAnyAction();                // 0x006622A0
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->IsDead(); }   // inline here (out-of-line copies e.g. 0x00428F60)
class CTCLook {
public:
    unsigned char       _pad_0x00[0x10];
    CThingCreatureBase* PParentCreature;         // +0x10
private:
    bool IsAbleToTurnDaBody() const;             // Ego_r ?IsAbleToTurnDaBody@CTCLook@@ABE_NXZ
};

bool CTCLook::IsAbleToTurnDaBody() const
{
    if (IS_THING_ALIVE(PParentCreature) && PParentCreature->IsPerformingAnyAction()) {
        if (PParentCreature->m_pActionController->IsBodyTurnLocked())
            return false;
    }
    return true;
}