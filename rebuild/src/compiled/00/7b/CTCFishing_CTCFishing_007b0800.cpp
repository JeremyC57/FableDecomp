// CTCFishing::CTCFishing(CThing*) -- CTCBase(thing), no fish, stage 0 not initialised, all counters/distances/score cleared, empty text, then the fishing def taken from the parent's definition
// Leaf: ??0CTCFishing
//
// Retail 0x007B0800, 127 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer<CThing>
// vtable 0x01238C6C), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// CDefClassBase::GetSubDef<CFishingDef> @ 0x007B2540 (manifest template name is a byte twin).
// Vtable 0x0126D5CC = CTCFishing.
// Layout = Ego_r.pdb CTCFishing (0x64, no retail shift): PFish +0xc, Stage +0x14, StageInitialised
// +0x18, FrameStageFinishes +0x1c, HookPresses +0x20, PressedThisFrame +0x24, LastFramePressed +0x28,
// Distance +0x2c, MaxDistance +0x30, Strain +0x34, FrameReelFinishes +0x38, Pulling +0x3c, Score +0x40,
// Text +0x44, TextTimer +0x48, InstructionsShown +0x4c, FrameStarted +0x50 and CurrentFishWeight +0x5c
// (both left uninitialised), ReelingInSoundID +0x54, FishEscapingSoundID +0x58, PDef +0x60; retail
// CThing def pointer object +0x70.
// Shape: declaration-order initialisers sharing one zero register (ebx; in this init list the three
// float zeros share it too), the Text ctor's pushes scheduled ahead of the stores; the body passes the
// PDef address formed for its zero store.
class CBaseObject;
class CThing;
class CFishingDef;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<const CFishingDef>* out);    // 0x007B2540
};

class CThing { public:
    unsigned char              _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                        // +0x70
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

class CTCFishing : public CTCBase { public:                               // vtable 0x0126D5CC
    CIntelligentPointer<CThing> PFish;                      // +0x0c
    long          Stage;                                    // +0x14 EFishingStage
    bool          StageInitialised;                         // +0x18
    long          FrameStageFinishes;                       // +0x1c
    long          HookPresses;                              // +0x20
    bool          PressedThisFrame;                         // +0x24
    long          LastFramePressed;                         // +0x28
    float         Distance;                                 // +0x2c
    float         MaxDistance;                              // +0x30
    float         Strain;                                   // +0x34
    long          FrameReelFinishes;                        // +0x38
    bool          Pulling;                                  // +0x3c
    long          Score;                                    // +0x40
    CCharString   Text;                                     // +0x44
    long          TextTimer;                                // +0x48
    bool          InstructionsShown;                        // +0x4c
    long          FrameStarted;                             // +0x50
    unsigned long ReelingInSoundID;                         // +0x54
    unsigned long FishEscapingSoundID;                      // +0x58
    long          CurrentFishWeight;                        // +0x5c
    CDefPointer<const CFishingDef> PDef;                    // +0x60
    virtual void v00();
    CTCFishing(CThing* thing);
};

CTCFishing::CTCFishing(CThing* thing)
    : CTCBase(thing)
    , PFish()
    , Stage(0)
    , StageInitialised(false)
    , FrameStageFinishes(0)
    , HookPresses(0)
    , PressedThisFrame(false)
    , LastFramePressed(0)
    , Distance(0.0f)
    , MaxDistance(0.0f)
    , Strain(0.0f)
    , FrameReelFinishes(0)
    , Pulling(false)
    , Score(0)
    , Text("", -1)
    , TextTimer(0)
    , InstructionsShown(false)
    , ReelingInSoundID(0)
    , FishEscapingSoundID(0)
    , PDef()
{
    ParentThing->PDef->GetSubDef(&PDef);
}