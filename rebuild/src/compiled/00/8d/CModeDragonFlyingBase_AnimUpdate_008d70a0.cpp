// CModeDragonFlyingBase::AnimUpdate(CAnimateThing&) -- keep the dragon's FLY_LOOP main anim and FLYING_LEG_POSE combination anim playing
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D70A0, 289 bytes, `ret 4`.
// Shape: the blend-in thing is a named re-read of ParentThing taken right after the FLY_LOOP lookup (retail loads it into
//   the dead `this` register ebx before the CAnimInit build); the combination anim found after AddCombinationAnim is a
//   named local, so its lookup call runs before the SetInternalAlpha argument pushes.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThing;
struct CAnimationEntry {
    long Index;                                // +0x00
    unsigned long GetAnimateThingFlags();      // 0x005DC390
    long GetTimeToBlendIn(CThing* thing);      // 0x005DC230
};
struct CThing { };
struct CThingCreatureBase : CThing {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};
struct CBlendPair {                            // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                             // 0x2c bytes
    long          Mode;                        // +0x00
    unsigned char AnimType;                    // +0x04
    bool          Flag05;                      // +0x05
    long          AnimIndex;                   // +0x08
    float         Scales[3];                   // +0x0c
    CBlendPair    Blend;                       // +0x18
    unsigned long Flags;                       // +0x20
    long          Arg24;                       // +0x24
    long          Percent;                     // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimSet {
    void SetInternalAlpha(long index, float alpha, bool b);   // 0x0070C280
};
struct CAnimateThing {
    unsigned char _pad00[0x04];
    void*         m_queueBegin;                // +0x04
    void*         m_queueEnd;                  // +0x08
    unsigned char _pad0c[0x50 - 0x0c];
    unsigned char CurrentAnimType;             // +0x50 (main CAnimSet)
    unsigned char CurrentAnimFlags;            // +0x51
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);   // 0x0070D580
    CAnimSet* FindCombinationAnim(long type);  // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
};
struct CModeBase {                             // CTCCreatureModeManager::CModeBase
    virtual void v00();
    CThingCreatureBase* ParentThing;           // +0x04
};
struct CModeDragonFlyingBase : CModeBase {
    void AnimUpdate(CAnimateThing& animate);
};

void CModeDragonFlyingBase::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = ParentThing;
    if (animate.m_queueBegin == animate.m_queueEnd || (animate.CurrentAnimFlags & 2) || animate.CurrentAnimType != 9) {
        CCharString flyLoop("FLY_LOOP");
        CAnimationEntry* entry = creature->GetAnimFromName(flyLoop, 0);
        CThingCreatureBase* parent = ParentThing;
        animate.SetMainAnim(CAnimInit(9, 0, entry->Index, 1, false, entry->GetAnimateThingFlags(), 100), entry->GetTimeToBlendIn(parent), 0);
    }
    CCharString legPose("FLYING_LEG_POSE");
    CAnimationEntry* pose = creature->GetAnimFromName(legPose, 0);
    if (pose->Index != -1 && animate.FindCombinationAnim(0x22) == 0) {
        animate.AddCombinationAnim(CAnimInit(0x22, 0, pose->Index, 0, true, 0, 100), 0, 0, false, false);
        CAnimSet* combination = animate.FindCombinationAnim(0x22);
        combination->SetInternalAlpha(0, 0.0f, false);
    }
}