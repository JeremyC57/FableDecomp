// CTCCreatureExpression::UpdateBlinking(CAnimateThing& animate, long) -- on a 1-in-50 world-seed roll start a blink: play a random loaded "ST_BLINK" animation as combination animation type 13; while blinking, stop once the blink animation is gone or has finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728FF0, 280 bytes (+ int3 padding), `ret 8`.
// Callees: CTCBase::GetGameSubsystem @ 0x00686D60 (the main game component; its WorldSeed +0x1613C), GFROR13 @ 0x00497890
//   (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the creature's virtual slot 73
//   (+0x124, has-animation test (const CCharString&, bool)), CThingCreatureBase::GetRandomLoadedAnimFromName(const
//   CCharString&, bool) @ 0x00662FA0, CAnimationEntry::GetAnimateThingFlags @ 0x005DC390, the 7-argument CAnimInit
//   constructor @ 0x005B33F4 (zeroes the blend pair then forwards to CAnimInit::Set 0x0070C050; manifest
//   "CAIStateGroup_WhisperRetreat::Initialise" is propagated), CAnimateThing::FindCombinationAnim(long) @ 0x0070C4E0,
//   AddCombinationAnim(const CAnimInit&, long, long, bool, bool) @ 0x0070D780, RemoveCombinationAnim @ 0x0070D8E0
//   (manifest "CSpellContainerList::AddChild" is propagated; named descriptively).
// Literal: "ST_BLINK" @ 0x012665BC.
// Layout = Ego_r.pdb CTCCreatureExpression (0x1c): ParentThing +0x04, ExpressionStates (retail 4-byte list) +0x0c,
//   Blinking +0x10; the combination anim's finished flag = bit 1 of +0x51 (named descriptively).
// Shape: the parent creature is a named local read first; the seed step is the landed inline GFNextRandomSeed
//   (0x00728000); the has-animation result is a short-lived named bool (ebx pushed around it only); each "ST_BLINK" is
//   an implicit CCharString conversion temporary; the start block falls through into the blinking test.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
long __fastcall GFROR13(unsigned long);  // 0x00497890
inline unsigned long GFNextRandomSeed(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
class CMainGameComponent { public:
    unsigned char _pad00[0x1613c];
    unsigned long WorldSeed;                            // +0x1613c
};
class CAnimationEntry { public:
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
class CAnimInit { public:                                      // 0x2c bytes
    unsigned char _body[0x2c];
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x005B33F4
};
struct CCombinationAnim {
    unsigned char _pad00[0x51];
    unsigned char Flags51;                              // +0x51 bit 1: finished
};
class CAnimateThing { public:
    CCombinationAnim* FindCombinationAnim(long type);   // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
    void RemoveCombinationAnim(CCombinationAnim* anim); // 0x0070D8E0
};
class CThingCreatureBase { public:
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
    virtual void v72();
    virtual bool HasAnimation(const CCharString& name, bool loadedOnly);   // slot 73 (+0x124)
public:
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString&, bool) const;   // 0x00662FA0
};
class CTCCreatureExpression { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                    // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool Blinking;                                      // +0x10
    CMainGameComponent* GetGameSubsystem();             // 0x00686D60 (CTCBase)
    void UpdateBlinking(CAnimateThing& animate, long);
};

void CTCCreatureExpression::UpdateBlinking(CAnimateThing& animate, long)
{
    CThingCreatureBase* creature = ParentThing;
    if (GFNextRandomSeed(&GetGameSubsystem()->WorldSeed) % 50 == 0 && !Blinking) {
        bool hasBlink = creature->HasAnimation("ST_BLINK", false);
        if (hasBlink) {
            const CAnimationEntry* entry = creature->GetRandomLoadedAnimFromName("ST_BLINK", false);
            if (entry) {
                CAnimInit init(0xd, 0, entry->AnimIndex, 1, false, entry->GetAnimateThingFlags(), 0x65);
                animate.AddCombinationAnim(init, 0, 0, false, false);
                Blinking = true;
            }
        }
    }
    if (Blinking) {
        CCombinationAnim* blink = animate.FindCombinationAnim(0xd);
        if (blink) {
            if (!(blink->Flags51 & 2))
                return;
            animate.RemoveCombinationAnim(blink);
        }
        Blinking = false;
    }
}