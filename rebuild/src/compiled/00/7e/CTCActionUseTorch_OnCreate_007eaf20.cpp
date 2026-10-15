// CTCActionUseTorch::OnCreate() -- base OnCreate, then TorchLit = bit 8 of the owner thing's virtual slot-48 flag word is clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EAF20, length 31 bytes.
// Leaf: OnCreate
// Callees: CTCActionUseBase::OnCreate @ 0x007EAC10 (non-virtual base call), the thing's virtual slot 48 (+0xc0) returning a
// flag word. Ego_r.pdb: `?OnCreate@CTCActionUseTorch@@UAEXXZ`, CTCActionUseTorch = CTCActionUseBase (0x14) + bool TorchLit +0x14.
// Shape: `shr eax,8 ; not al ; and al,1` is `!((flags >> 8) & 1)` on a SCALAR return. A struct-of-bitfields return gives
// the same bit code, but a member function returns any struct through a hidden pointer (`lea edx,[esp+4] ; push edx`).
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual unsigned long GetStateFlags();     // slot 48 (+0xc0)
};

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

struct CTCActionUseBase : CTCBase {
    unsigned char _pad0c[8];                   // +0x0c PActionUseDef, +0x10 Usable
    virtual void OnCreate();                   // 0x007EAC10
};

struct CTCActionUseTorch : CTCActionUseBase {
    bool TorchLit;                             // +0x14
    virtual void OnCreate();
};

void CTCActionUseTorch::OnCreate()
{
    CTCActionUseBase::OnCreate();
    TorchLit = !((ParentThing->GetStateFlags() >> 8) & 1);
}