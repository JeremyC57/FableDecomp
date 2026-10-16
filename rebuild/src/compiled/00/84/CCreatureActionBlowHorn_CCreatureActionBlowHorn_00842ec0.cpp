// CCreatureAction_BlowHorn::CCreatureAction_BlowHorn(CThingCreatureBase*, CThing*) -- base CCreatureActionBase(owner, -1.0, 1.0, the owner's "BLOW_KNOTHOLE_HORN" animation, the owner's "HORN_BLOW" sound pair, 0, 0), PObjectToUse = the horn thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842EC0, length 163 bytes (+ int3 padding), `ret 8`. Vtable 0x0127428C;
// PObjectToUse is a CIntelligentPointer<CThing> (vtable 0x01238C6C) at retail +0xa8 (PDB +0xb4).
// Callees: CCharString(const char*, long) 0x0099EBF0 ("HORN_BLOW" 0x012743B8, "BLOW_KNOTHOLE_HORN" 0x012743A4), the owner's
// virtual slot 89 (+0x164: the sound pair for a name, hidden return pointer), the animation lookup 0x006924B0 (`__fastcall`),
// CCreatureActionBase::CCreatureActionBase 0x00693B30, ~CCharString 0x0099EAE0 (twice), CBaseIntelligentPointer ctor 0x00A01B10,
// SetPItem 0x00A01B90.
// Shape: both literals are implicit CCharString conversion temporaries, built before any argument is pushed in right-to-left
// argument order (the sound name first) and destroyed after the base call in reverse order; the pointer member is constructed
// from the thing in the init list (inline converting ctor: base ctor, derived vtable, SetPItem).
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CBaseObject;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {                             // two sound ids (user ctor: returned through a hidden pointer)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CThingCreatureBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual CSoundPair GetSoundPair(const CCharString& name);   // slot 89 (+0x164)
};
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCreatureAction_BlowHorn : CCreatureActionBase {   // vtable 0x0127428C
    CIntelligentPointerThing PObjectToUse;     // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_BlowHorn(CThingCreatureBase* owner, CThing* horn);
};

CCreatureAction_BlowHorn::CCreatureAction_BlowHorn(CThingCreatureBase* owner, CThing* horn)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationEntry(owner, "BLOW_KNOTHOLE_HORN"), owner->GetSoundPair("HORN_BLOW"), 0, 0)
    , PObjectToUse(horn)
{
}