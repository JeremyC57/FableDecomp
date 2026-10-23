// Grade: DIFFER(531v550): register allocation -- retail holds the sub-def pointer in ebx across the block and materialises the explode condition in a frame byte (ours: bool in bl, def reloaded), and builds the knockdown with the creature in ebx and the target spilled (ours: target in ebp); the remaining size is the resulting disp32 frame operands
// CTCBoss::KillHelpers() -- every live helper creature either explodes (killed outright when its sub-def says EXPLODE_ON_KILLING_BLOW) or is drained past zero health and knocked down; then each helper slot is cleared
// Leaf: KillHelpers
// Retail 0x007AF490, 560 bytes, `ret`. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
// SetPItem 0x00A01B90 / dtor 0x00A01C10, CDefClassBase::GetSubDef 0x00669580, CCharString 0x0099EBF0 / dtor 0x0099EAE0,
// CCRC::CalcAndLookup 0x00669D30, CThing::Kill 0x004C9B80, the creature's virtual +0xEC, CThing::GetPosition
// 0x004C73D0, CCombatAction_GenericStrikeResponseKnockdown 0x008679C0, CThingCreatureBase::SetCurrentAction 0x006644F0,
// ~CCombatActionBase 0x0062E900. Constant 1.0f 0x0122DED8; CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0.
// Layout: Ego_r.pdb CTCBoss with the retail 12-byte vector: HelperCreatures +0x30; CThing def +0x70, health +0xB4,
// dead bit +0x91; knockdown action as the landed 0x00667310 (base 0x114, AnimName +0x114, PThingThatHit +0x118).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CSubDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSubDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the landed 0x008C8380 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSubDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSubDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CSubDef>* out);          // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
// Stand-in body (the real one @ 0x00669580 is out of line): it writes the out slot only, so the def pointer's address
// does not escape the call (the landed 0x008C8380 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSubDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void ChangeHealth(float delta, bool fromHero);   // +0xEC
    unsigned char  _pad04[0x70 - 0x04];
    CDefClassBase* m_pDef;                              // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    unsigned char  _pad92[0xb4 - 0x92];
    float          Health;                              // +0xB4
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    bool SetCurrentAction(const struct CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                               // 0x0062E900
};
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {
    CCharString             AnimName;                   // +0x114
    CBaseIntelligentPointer PThingThatHit;              // +0x118
    unsigned char           _hitInfo[0x1c];             // +0x120
    CCombatAction_GenericStrikeResponseKnockdown(CThing* creature, CThing* hitter, const C3DVector& point,
                                                 const C3DVector& direction);   // 0x008679C0
};
struct CHelperVector {
    CIntelligentPointerCreature* _M_start;
    CIntelligentPointerCreature* _M_finish;
    CIntelligentPointerCreature* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CIntelligentPointerCreature* begin() { return _M_start; }
    CIntelligentPointerCreature& operator[](unsigned long i) { return *(begin() + i); }
};
inline bool IsAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1); }
struct CTCBoss {
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x30 - 0x04];
    CHelperVector HelperCreatures;                      // +0x30
    void KillHelpers();
};

void CTCBoss::KillHelpers()
{
    for (unsigned long i = 0; i < HelperCreatures.size(); ++i) {
        if (IsAlive(HelperCreatures[i])) {
            CDefPointer<CSubDef> def;
            long value;
            CDefClassBase* thingDef = HelperCreatures[i].GetPItem()->m_pDef;
            if (thingDef->GetSubDef(&def) && def->CalcAndLookup("EXPLODE_ON_KILLING_BLOW", &value)) {
                HelperCreatures[i].GetPItem()->Kill(true);
            } else {
                HelperCreatures[i].GetPItem()->ChangeHealth(-(HelperCreatures[i].GetPItem()->Health + 1.0f), true);
                C3DVector up(0.0f, 0.0f, 1.0f);
                CThing* creature = HelperCreatures[i].GetPItem();
                CThing* target = HelperCreatures[i].GetPItem();
                target->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(
                    creature, 0, HelperCreatures[i].GetPItem()->GetPosition(), up));
            }
        }
        HelperCreatures[i] = CIntelligentPointerCreature(0);
    }
}
