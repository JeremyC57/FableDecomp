// CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(CThingCreatureBase*, CThing*, const CDefPointer<const CExpressionDef>&, long, long, long, long)
//   -- start an expression: base action on the def's Animation, then (non-zero duration) the extendable "Into"/"ToOther" variants
// Retail: Fable.exe (TLC, VC7.1) @ 0x008430B0, 608 bytes, `ret 1Ch`. Vtable 0x012743DC (CCreatureAction_PerformExpression,
//   same store as the landed copy ctor 0x0084BD40); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Callees: CDefStringTable::GetString(CDefString) 0x009D49B0 on the global table 0x013CA828, GetRandomLoadedAnim
//   (__fastcall) 0x006924B0, CCreatureActionBase ctor 0x00693B30, CCharString dtor 0x0099EAE0, CBaseIntelligentPointer
//   ctor/GetPItem/SetPItem 0x00A01B10/0x00A01B50/0x00A01B90, CDefString::operator!=(const char*) 0x005FA740,
//   CCreatureActionBase::SetAnimation 0x00693030.
// Layout: engine/CCreatureAction_PerformExpression.h is a flat PDB-offset view (base 0xb4); the retail CCreatureActionBase
//   is 0xa8 bytes (landed 0x0084BD40), so members sit at PDB - 0x0c. CExpressionDef: retail offsets are PDB - 4
//   (Animation +0x3c, AnimationInto +0x40, AnimationToOther +0x4c, AnimationToOtherInto +0x50, Extendable +0x84).
struct CBaseObject;
struct CThing;
struct CThingCreatureBase;
struct CAnimationEntry;

struct CCharString {
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefString {
    long TablePos;
    bool operator!=(const char* text) const;            // 0x005FA740
    operator CCharString() const;
};

struct CDefStringTable {
    CCharString GetString(CDefString name);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
inline CDefString::operator CCharString() const { return g_DefStringTable.GetString(*this); }

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
};

struct CDefPointeeBase { virtual void VFunc00(); long RefCount; };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++((CDefPointeeBase*)Object)->RefCount; }
    T* operator->() const { return Object; }
};

struct CExpressionDef {
    unsigned char _head[0x3c];
    CDefString Animation;                               // +0x3c
    CDefString AnimationInto;                           // +0x40
    CDefString AnimationLoop;                           // +0x44
    CDefString AnimationOutOf;                          // +0x48
    CDefString AnimationToOther;                        // +0x4c
    CDefString AnimationToOtherInto;                    // +0x50
    unsigned char _mid[0x84 - 0x54];
    bool Extendable;                                    // +0x84
};

struct CCreatureActionBase {
    virtual ~CCreatureActionBase();
    long _04;                                           // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;  // +0x08
    long TotalTime;                                     // +0x10
    unsigned char _body[0xa8 - 0x14];
    CCreatureActionBase(CThingCreatureBase* owner, float duration, float fraction, const CAnimationEntry& entry,
                        const CSoundPair& sounds, long group, long priority);   // 0x00693B30
    void SetAnimation(const CAnimationEntry& entry, float duration, float fraction);   // 0x00693030
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PCreature.GetPItem(); }
};

struct CCreatureAction_PerformExpression : CCreatureActionBase {
    CDefPointer<const CExpressionDef> PExpressionDef;   // +0xa8
    CIntelligentPointer<CThing> PPerformToThing;        // +0xac
    long ExpressionItemDefIndex;                        // +0xb4
    bool EmoteIconAdded;                                // +0xb8
    bool ReplacedAction;                                // +0xb9
    bool Extended;                                      // +0xba
    bool ButtonHeld;                                    // +0xbb
    unsigned long SoundEffectID;                        // +0xbc
    long ExtraData1;                                    // +0xc0
    long ExtraData2;                                    // +0xc4
    virtual ~CCreatureAction_PerformExpression();
    CCreatureAction_PerformExpression(CThingCreatureBase* owner, CThing* performTo,
                                      const CDefPointer<const CExpressionDef>& def, long itemDefIndex,
                                      long duration, long extra1, long extra2);
};

CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(CThingCreatureBase* owner, CThing* performTo,
        const CDefPointer<const CExpressionDef>& def, long itemDefIndex, long duration, long extra1, long extra2)
    : CCreatureActionBase(owner, (float)duration, 0.0f,
                          *GetRandomLoadedAnim(owner, def->Animation), CSoundPair(), 0, 0),
      PExpressionDef(def), PPerformToThing(performTo), ExpressionItemDefIndex(itemDefIndex), EmoteIconAdded(false),
      ReplacedAction(false), Extended(false), ButtonHeld(false), SoundEffectID(0), ExtraData1(extra1), ExtraData2(extra2)
{
    if (duration == 0.0f) {
        TotalTime = 0;
        return;
    }
    if (PExpressionDef->Extendable) {
        if (PPerformToThing.GetPItem() && PExpressionDef->AnimationToOtherInto != "") {
            SetAnimation(*GetRandomLoadedAnim(GetCreature(), PExpressionDef->AnimationToOtherInto), -1.0f, 0.0f);
            Extended = true;
        } else if (PExpressionDef->AnimationInto != "") {
            SetAnimation(*GetRandomLoadedAnim(GetCreature(), PExpressionDef->AnimationInto), -1.0f, 0.0f);
            Extended = true;
        } else {
            SetAnimation(*GetRandomLoadedAnim(GetCreature(), PExpressionDef->Animation), -1.0f, 0.0f);
        }
    } else if (PPerformToThing.GetPItem() && PExpressionDef->AnimationToOther != "") {
        SetAnimation(*GetRandomLoadedAnim(GetCreature(), PExpressionDef->AnimationToOther), -1.0f, 0.0f);
    }
}
