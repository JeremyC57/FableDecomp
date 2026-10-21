// CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(CThingCreatureBase* owner, CThing* performTo, const CDefPointer<const CExpressionDef>& def, long itemDefIndex, bool emoteIconAdded, long extraData, unsigned long soundEffectID, long interruptionGroup) -- base CCreatureActionBase(owner, -1.0f, 0.0f, the owner's animation named by the expression def's AnimName string, zeroed sound, interruptionGroup, 0), copy the def and the members, then when performing to a thing swap in the targeted animation (TargetedAnimName if set, else the UntargetedAnimName) with SetAnimation(anim, -1.0f, 0.0f)
// Grade: DIFFER(335v347) -- retail homes the GetString temporary for the base-ctor argument in the dead `def` argument slot ([esp+20h]) and zeroes the CSoundPair after its address push; ours gives the string its own frame word (the "temporary over a dead argument slot" gap: would need same-TU stand-ins for GetString / the animation lookup) | tried: init-list form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843310, length 347 bytes (+ int3 padding), `ret 20h`. Vtable 0x012744FC (RTTI:
// CCreatureAction_PerformExpressionExtended; the manifest/PDB name "CCreatureAction_PerformExpression" is the base-class label).
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828 (CCharString by hidden pointer), the owner's
// animation lookup by name @ 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase @ 0x00693B30, ~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer ctor @ 0x00A01B10 and SetPItem @ 0x00A01B90 (PPerformToThing, vtable 0x01238C6C), GetPItem @ 0x00A01B50,
// CDefString::operator!=(const char*) const @ 0x005FA740 (the def's name handles at +0x54 and +0x44 against ""),
// CCreatureActionBase::SetAnimation(CAnimationEntry*, float, float) @ 0x00693030.
// Layout: retail CCreatureActionBase is 0xa8 bytes (the PDB header is 12 larger): PExpressionDef +0xa8, PPerformToThing +0xac,
// ExpressionItemDefIndex +0xb4, EmoteIconAdded +0xb8, SoundEffectID +0xbc, ReplacedAction +0xc0 (false), ExtraData +0xc4.
// Shape: the GetString temporary for the base argument lives in the dead `def` argument slot; the CDefPointer copy is the inline
// add-ref; the animation id is a named long set in either branch of the if/else-if (an early return when neither name is set) and
// the second GetString/lookup/SetAnimation sequence is shared.
struct CCharString {
    void* m_pData;
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefString {                                    // 4-byte def-name handle
    long m_tablePos;
    bool operator!=(const char* text) const;           // 0x005FA740
};
struct CDefStringTable {
    CCharString GetString(long id);                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CAnimationEntry* __fastcall GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0
struct CExpressionDef {
    virtual void v00();
    long RefCount;                                     // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long AnimName;                                     // +0x3c string id
    unsigned char _pad40[0x44 - 0x40];
    CDefString UntargetedAnimName;                     // +0x44
    unsigned char _pad48[0x54 - 0x48];
    CDefString TargetedAnimName;                       // +0x54
};
struct CDefPointer {                                   // CDefPointer<const CExpressionDef>
    CExpressionDef* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    CExpressionDef* operator->() const { return Object; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                     // +4
    CBaseIntelligentPointer();                         // 0x00A01B10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CCreatureActionBase {                           // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
    void SetAnimation(CAnimationEntry* animation, float a, float b);   // 0x00693030
};
struct CCreatureAction_PerformExpressionExtended : CCreatureActionBase {   // vtable 0x012744FC
    CDefPointer              PExpressionDef;           // +0xa8
    CIntelligentThingPointer PPerformToThing;          // +0xac
    long                     ExpressionItemDefIndex;   // +0xb4
    bool                     EmoteIconAdded;           // +0xb8
    unsigned long            SoundEffectID;            // +0xbc
    bool                     ReplacedAction;           // +0xc0
    long                     ExtraData;                // +0xc4
    CCreatureAction_PerformExpressionExtended(CThingCreatureBase* owner, CThing* performTo, const CDefPointer& def, long itemDefIndex,
                                              bool emoteIconAdded, long extraData, unsigned long soundEffectID, long interruptionGroup);
};

CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(CThingCreatureBase* owner, CThing* performTo,
        const CDefPointer& def, long itemDefIndex, bool emoteIconAdded, long extraData, unsigned long soundEffectID, long interruptionGroup)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnimation(owner, g_DefStringTable.GetString(def->AnimName)), CSoundPair(), interruptionGroup, 0),
      PExpressionDef(def), PPerformToThing(performTo), ExpressionItemDefIndex(itemDefIndex), EmoteIconAdded(emoteIconAdded),
      SoundEffectID(soundEffectID), ReplacedAction(false), ExtraData(extraData)
{
    if (PPerformToThing.GetPItem()) {
        long animName;
        if (PExpressionDef->TargetedAnimName != "")
            animName = PExpressionDef->TargetedAnimName.m_tablePos;
        else if (PExpressionDef->UntargetedAnimName != "")
            animName = PExpressionDef->UntargetedAnimName.m_tablePos;
        else
            return;
        SetAnimation(GetRandomLoadedAnimation((CThingCreatureBase*)owner, g_DefStringTable.GetString(animName)), -1.0f, 0.0f);
    }
}
