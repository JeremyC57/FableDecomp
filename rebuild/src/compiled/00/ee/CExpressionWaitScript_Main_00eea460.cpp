// CExpression_WaitScript::Main() -- the hero "wait" expression quest: register a conscious-hero predicate, run a frame, take the hero under scripted control (retrying a frame at a time), then inside an empty movie sequence fade out, pause, kill the follow thread, deactivate the Expression_Follow quest, reset the camera, pause, fade back in and deactivate this quest later
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEA460, length 0x218 bytes (+ int3 padding), no EH frame.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840, CScriptBase::SetPredicate 0x00CB7920 /
//   IsActiveThreadTerminating 0x00CB7940 / KillSpawnedFunction 0x00CB7AA0, CBaseObject ctor 0x0099A380 / dtor 0x0099A430,
//   the scripted-thing handle's out-of-line dtor 0x007E74D0, the movie handle's PImp dtor 0x006E7AB0,
//   CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   70 GetHero, 280 DeactivateQuest, 281 DeactivateQuestLater, 370 StartMovieSequence, 373 FadeScreenOut, 374 FadeScreenIn,
//   376 Pause, 416 CameraDefault, 655 GetActiveQuestName. Literals "" 0x0122D70E, "FollowThread" 0x012F55E8,
//   "Expression_Follow" 0x01259170. Manifest labels for 0x00CB7920 / 0x0099EAE0 / 0x004AA840 are propagated.
// Layout: the retail quest-script base (as the landed CQ_* rows): CScriptBase 0x40 bytes, the interface pointer at +0x40.
// Shape: the predicate (CIsThingConscious, vtable 0x012C2FE8) is a scoped object; the hero handle's dtor is out of line (both
//   exits share one call), the movie handle's compiler-generated dtor is expanded inline; the CRGBColour argument is built in its
//   stack slot in R +2, G +1, B +0, A +3 order, which needs the value ctor written as body assignments R, G, B, A (member
//   initialisers store in declaration order) plus a __forceinline user copy ctor; string literals convert into a CCharString temporary before the pushes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};

class CRGBColour { public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};

class CScriptThing { public:                                      // 12 bytes
    unsigned char _body[0xc - 4];
    CScriptThing(const CScriptThing& other);               // 0x004ABE90
    virtual ~CScriptThing();                                       // 0x004AA840
};

struct CIsThingConscious {                                 // vtable 0x012C2FE8
    CScriptThing Thing;                                    // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingConscious() { }
    virtual bool operator()() const;
};

class CBaseObject { public:
    void* IntelligentPointer;                              // +0x04
protected:
    CBaseObject();                                         // 0x0099A380
public:
    virtual ~CBaseObject();                                // 0x0099A430
};

class CScriptGameResourceObjectBase : public CBaseObject { public:       // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() { }
};

struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;                             // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();         // 0x007E74D0
    virtual void v01();
};

struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) { }
    ~CMovieImpPointer();                                   // 0x006E7AB0
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;                                 // +0x08
    virtual void v01();
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                     // slots 0..6
    virtual void NewScriptFrame() const;                                              // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                               // slots 9..69
    virtual const CScriptThing& GetHero() const;                                      // slot 70 (+0x118)
    VPAD128(ra) VPAD64(rb) VPAD8(rc) VPAD1(rd)                                        // slots 71..271
    VPAD8(rf)                                                                         // slots 272..279
    virtual void DeactivateQuest(const CCharString& name, unsigned long flags) const;        // slot 280 (+0x460)
    virtual void DeactivateQuestLater(const CCharString& name, unsigned long flags) const;   // slot 281 (+0x464)
    VPAD64(sa) VPAD16(sb) VPAD4(sc) VPAD4(sd)                                         // slots 282..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD2(ta)                                                                         // slots 371..372
    virtual bool FadeScreenOut(float time, float hold, CRGBColour colour);            // slot 373 (+0x5d4)
    virtual void FadeScreenIn();                                                      // slot 374 (+0x5d8)
    VPAD1(ua)                                                                         // slot 375
    virtual void Pause(float seconds) const;                                          // slot 376 (+0x5e0)
    VPAD32(va) VPAD4(vb) VPAD2(vc) VPAD1(vd)                                          // slots 377..415
    virtual void CameraDefault();                                                     // slot 416 (+0x680)
    VPAD128(wa) VPAD64(wb) VPAD32(wc) VPAD8(wd) VPAD4(we) VPAD2(wf)                   // slots 417..654
    virtual CCharString GetActiveQuestName() const;                                   // slot 655 (+0xa3c)
};

class CScriptBase { public:                                       // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    void SetPredicate(CIsThingConscious& predicate);       // 0x00CB7920
    bool IsActiveThreadTerminating();                      // 0x00CB7940
    void KillSpawnedFunction(const CCharString& name, unsigned long id);   // 0x00CB7AA0
};

struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;                             // +0x40
};

struct CExpression_WaitScript : CQuestScriptBase {
    void Main();
};

void CExpression_WaitScript::Main()
{
    {
        CIsThingConscious predicate(GSI->GetHero());
        SetPredicate(predicate);
    }
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase hero;
    while (!GSI->StartScriptingEntity(GSI->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    GSI->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    GSI->Pause(1.0f);
    KillSpawnedFunction("FollowThread", 0);
    GSI->DeactivateQuest("Expression_Follow", 0);
    GSI->CameraDefault();
    GSI->Pause(1.0f);
    GSI->FadeScreenIn();
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}