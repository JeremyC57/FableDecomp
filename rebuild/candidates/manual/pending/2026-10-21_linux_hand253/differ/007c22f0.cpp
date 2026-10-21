// Grade: DIFFER(413v397) -- the Init call: retail homes the accuracy float in the dead 'normal' argument slot and the melee out-def in the string's slot, copies the by-value def with `inc [edi+4]` and releases the melee def with `dec [ecx+4]; jne`; ours keeps accuracy in the frame, re-loads the count around inc, and releases through the DecRef form
// NThingShot::CTCShotDefinitionBase::OnHitByMelee(CThing*, long, const C3DVector&, const C3DVector&, long, long) -- a shot struck by a melee blow: a physical striker becomes the shot's best collision (position, normal, type 1) and the collision is processed at once; otherwise, when the shot definition (or, for a multi-arrow shot, its ability sub-definition) allows deflection, a CTCShotDefinitionThrow component is added and initialised to fly back from the striker
// Leaf: ?OnHitByMelee@CTCShotDefinitionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C22F0, 395 bytes (`ret 18h`); vtable slot 29 (CTCShotDefinitionThrow 0x0123CDC4).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CTCShotDefinitionBase::OnCollisionProper(bool*) 0x007C1000;
//          CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) 0x007BE7EA; CCharString(const char*, long) 0x0099EBF0 /
//          ~CCharString 0x0099EAE0 ("CTCShotDefinitionThrow" 0x0123CDA8); CThing::AddTC(const CCharString&, bool,
//          CTCParamsBase*) 0x004C9D60; CTCShotDefinitionBase::Init(CDefPointer<CThingShotDef const>, CThing*, CThing*, CThing*,
//          float, float, float, CDefPointer<CMeleeCombatAbilityDef const>*) 0x007C0690.
// Layout (Ego_r.pdb CTCShotDefinitionBase, retail PDB - 4 after the 12-byte retail vector): ParentThing +0x04, PShooter +0x28,
//          PShotDef +0x38, BestCollisionPos +0x70, BestCollisionNormal +0x7c, PBestCollisionThing +0x88, BestCollisionType +0x90,
//          MultiArrowShotType +0x98. Retail CThing interface bitset +0x20 (type 0x7d = word +0x2c bit 29). The shot def
//          (descriptive): deflectable +0x82, accuracy +0x84; the ability def: deflectable +0x4c.
struct C3DVector { float X, Y, Z; };
struct CBaseObject;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCParamsBase;
struct CTCShotDefinitionBase;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    bool HasTC(long type) const { return (m_presentBits[type >> 5] >> (type & 31)) & 1; }
    CTCShotDefinitionBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    void operator=(const CIntelligentPointerThing& other) { CBaseObject* item = (CBaseObject*)other.GetPItem(); SetPItem(item); }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CAbilityDef : CDefVirtual {
    long m_count;                               // +0x04
    void AddRef() { ++m_count; }
    void Release() { if (--m_count == 0) Destroy(); }
    unsigned char _pad08[0x4c - 0x08];
    bool CanBeDeflected;                        // +0x4c
};
struct CThingShotDef : CDefVirtual {
    long m_count;                               // +0x04
    void AddRef() { ++m_count; }
    void Release() { if (--m_count == 0) Destroy(); }
    unsigned char _pad08[0x82 - 0x08];
    bool CanBeDeflected;                        // +0x82
    unsigned char _pad83[0x84 - 0x83];
    float Accuracy;                             // +0x84
};
struct CMeleeCombatAbilityDef : CDefVirtual {
    long m_count;                               // +0x04
    void AddRef() { ++m_count; }
    void Release() { if (--m_count == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    CAbilityDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);    // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef(CDefPointer<CAbilityDef>* out);   // 0x007BE7EA (GetSubDef<CAbilityDef>)
};
// Stand-in for the same-module GetSubDef<CAbilityDef> body (0x007BE7EA, not graded here), as in the landed GetTotalWinnings
// 0x005EA310: compiled above the caller without inlining so VC7.1 sees what it writes.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out)
{
    long* index = GetSubDefIndex("CAbilityDef");
    if (index == 0)
        return false;
    CAbilityDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out->Object != def) {
        if (out->Object)
            out->Object->Release();
        out->Object = def;
        if (def)
            def->AddRef();
    }
    return true;
}
#pragma auto_inline(on)
struct CTCShotDefinitionBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    CIntelligentPointerThing PShooter;          // +0x28
    unsigned char _pad30[0x38 - 0x30];
    CDefPointer<CThingShotDef> PShotDef;        // +0x38
    unsigned char _pad3c[0x70 - 0x3c];
    C3DVector BestCollisionPos;                 // +0x70
    C3DVector BestCollisionNormal;              // +0x7c
    CIntelligentPointerThing PBestCollisionThing;   // +0x88
    long BestCollisionType;                     // +0x90
    unsigned char _pad94[0x98 - 0x94];
    bool MultiArrowShotType;                    // +0x98
    void OnCollisionProper(bool* collided);     // 0x007C1000
    void Init(CDefPointer<CThingShotDef> def, CThing* target, CThing* thing2, CThing* shooter, float speed, float accuracy,
              float delay, CDefPointer<CMeleeCombatAbilityDef>* ability);   // 0x007C0690
    virtual void OnHitByMelee(CThing* striker, long unused, const C3DVector& pos, const C3DVector& normal, long unused2, long unused3);
};

void CTCShotDefinitionBase::OnHitByMelee(CThing* striker, long, const C3DVector& pos, const C3DVector& normal, long, long)
{
    bool physical = striker->HasTC(0x7d);
    if (physical) {
        BestCollisionPos = pos;
        BestCollisionNormal = normal;
        BestCollisionType = 1;
        PBestCollisionThing = CIntelligentPointerThing(striker);
        bool collided;
        OnCollisionProper(&collided);
        return;
    }
    CDefPointer<CThingShotDef> def = PShotDef;
    bool deflectable = def->CanBeDeflected;
    if (MultiArrowShotType) {
        CDefPointer<CAbilityDef> ability;
        ((CDefClassBase*)PShotDef.Object)->GetSubDef(&ability);
        deflectable = ability->CanBeDeflected;
    }
    if (deflectable) {
        CThing* shooter = PShooter.GetPItem();
        float accuracy = PShotDef->Accuracy;
        CTCShotDefinitionBase* thrown;
        {
            CCharString name("CTCShotDefinitionThrow");
            thrown = ParentThing->AddTC(name, true, 0);
        }
        CDefPointer<CMeleeCombatAbilityDef> melee;
        thrown->Init(def, striker, 0, shooter, 100.0f, accuracy, 0.0f, &melee);
    }
}
