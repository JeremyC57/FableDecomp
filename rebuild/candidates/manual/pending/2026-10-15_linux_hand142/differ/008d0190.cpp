// CGameScriptThing::FrameUpdate() -- runs the per-frame virtual update (slot 71), then, when the scripted thing is alive and placed, caches its position (+0x28) and XY angle (+0x40)
// Grade: DIFFER(ours v retail) -- 112v112, register swap only: retail keeps this in edi and &PThing in esi, ours the reverse | tried: direct member chain, inline GetPosition/GetAngleXY accessors (fixed the vcall load), inline IsThingAlive helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0190, length 112 bytes.
// Callees: this's virtual slot 71 (+0x11C), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (five calls), the placement
//          component's virtual GetAngleXY (slot 63, +0xFC, float).
// Shape: the thing's placement record at +0x60 is re-fetched through GetPItem for every use; its position (+0x0c) is
// copied dword by dword through a `lea edx,[edi+28h]` destination (plain C3DVector assignment from a reference).
struct C3DVector {
    float X, Y, Z;
};
struct CThingPlacement_Retail {                    // the thing's placement / physics record (+0x60)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;              // slot 63 (+0xFC)
    unsigned char _pad04[8];
    C3DVector     m_position;                      // +0x0c
};
struct CThing_Retail {
    unsigned char  _pad00[0x60];
    CThingPlacement_Retail* m_pPlacement;          // +0x60
    unsigned char  _pad64[0x2d];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    const C3DVector& GetPosition() const { return m_pPlacement->m_position; }   // inline twin of 0x004C73D0
    float GetAngleXY() const { return m_pPlacement->GetAngleXY(); }
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void*          m_pVtbl;
    void*          m_pItem;
    CThing_Retail* GetPItem() const;               // 0x00A01B50
};
struct CGameScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void UpdateScript();                   // slot 71 (+0x11C)
    unsigned char           _pad04[8];
    CBaseIntelligentPointer PThing;                // +0x0c CIntelligentPointer<CThing>
    unsigned char           _pad14[0x14];
    C3DVector               Pos;                   // +0x28
    C3DVector               HomePos;               // +0x34
    float                   AngleXY;               // +0x40
    void FrameUpdate();
};

void CGameScriptThing::FrameUpdate()
{
    UpdateScript();
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead && PThing.GetPItem()->m_pPlacement) {
        Pos = PThing.GetPItem()->GetPosition();
        AngleXY = PThing.GetPItem()->GetAngleXY();
    }
}
