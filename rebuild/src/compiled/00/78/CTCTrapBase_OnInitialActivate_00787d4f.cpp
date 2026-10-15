#pragma optimize("s", on)
// CTCTrapBase::OnInitialActivate() -- fetches the trap def from the owner's def, caches the trap type, hands an optional def value to the owner's +0x60 object, runs InitTrap and sets a flag bit on the owner's +0x64 component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787D4F, length 69 bytes. Size TU: the flag set is a read-modify-write
// `or byte ptr [eax+59h],2` only under `optimize("s",on)` (speed gives mov/or/mov) -- the pragma also fixes the eax/edi colouring.
// Callees: CDefClassBase::GetSubDef<CTrapDef>(CDefPointer<const CTrapDef>*) @ 0x00788E53 (on the owner thing's def
// at +0x70; the manifest's "GetSubDef<CAbilityDef>" template argument is propagated), virtual slot 85 (+0x154) of the
// owner's +0x60 object, the virtual InitTrap() (slot 28, +0x70) on this.
// Shape: the def pointer member's address stays in edi (`lea edi,[esi+54h]`) and is re-read after the call; the
// owner (+0x04) is re-read for each use. `or byte ptr [eax+59h],2` is a flag bit set on the +0x64 component.
// Layout: CTCTrapBase.h is the PDB layout (PTrapDef_Object +0x60, TrapType +0x68); the retail constructor 0x0078918D
// puts them at +0x54 / +0x5C (12-byte STLport vectors), so a local model is used.
struct CTrapDef {
    unsigned char _pad[0x38];
    long          TrapType;                 // +0x38
    unsigned char _pad3c[0x10];
    long          OwnerValue;               // +0x4C (handed to the owner's +0x60 object when non-zero)
};

struct CDefPointerTrapDef {
    CTrapDef* Object;
};

struct CDefClassBase {
    bool GetSubDef(CDefPointerTrapDef* out);   // 0x00788E53
};

struct COwnerObject60 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84();
    virtual void Receive(long value);       // slot 85 (+0x154)
};

struct CComponentFlags {
    unsigned char _pad[0x59];
    unsigned char Flag0 : 1;                // +0x59 bit 0
    unsigned char Flag1 : 1;                // +0x59 bit 1 (set here)
};

struct CThing {
    unsigned char    _pad[0x60];
    COwnerObject60*  m_p60;                 // +0x60
    CComponentFlags* m_p64;                 // +0x64
    unsigned char    _pad68[0x8];
    CDefClassBase*   Def;                   // +0x70
};

struct CTCTrapBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void InitTrap();                // slot 28 (+0x70)
    CThing*            Thing;               // +0x04
    unsigned char      _pad_0x08[0x4C];
    CDefPointerTrapDef PTrapDef;            // +0x54
    unsigned char      _pad_0x58[0x4];      // +0x58 Active, Triggered, LastTriggerOnProximity
    long               TrapType;            // +0x5C
    const CTrapDef& GetTrapDef() const { return *PTrapDef.Object; }
    void OnInitialActivate();
};

void CTCTrapBase::OnInitialActivate()
{
    Thing->Def->GetSubDef(&PTrapDef);
    TrapType = GetTrapDef().TrapType;
    if (GetTrapDef().OwnerValue != 0)
        Thing->m_p60->Receive(GetTrapDef().OwnerValue);
    InitTrap();
    Thing->m_p64->Flag1 = 1;
}