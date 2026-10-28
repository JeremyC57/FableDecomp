// CAIStateGroup_GameMaster::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- runs the go-to-pub initialisation, then resets the react timer, the follow flag and the pub-thing pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094C7E0, length 102 bytes (+ int3 padding), `ret 8`.
// Callees: CAIStateGroup_GoToPub::InitStateGroup @ 0x00947DB0 (the base implementation, both arguments forwarded),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/90/50/0x00A01C10.
// Shape: base call first (arguments loaded before `push esi`), then TimeNextReactToPlayer (+0xa8) and
// WasAllowedToFollowPlayer (+0xac) cleared with immediates, then `PBaseThing = CIntelligentPointer<..>(0)` -- a
// frame temporary through the inline converting constructor (vtable 0x01244344), the inline operator= and the
// base destructor.
// Model: local (no engine header for these classes; GameMaster member names and offsets from struct_layouts_egor.tsv,
// whose CAIStateGroup_GoToPub is 0xa8 bytes like retail).
class CThing;
class CWorld;
class CThingAICreature;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<..>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointerThing PBaseThing;            // +0x14
    unsigned char _pad_0x1c[0xa8 - 0x1c];           // +0x1c..+0xa7
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);   // 0x00947DB0
};

struct CAIStateGroup_GameMaster : CAIStateGroup_GoToPub {
    long TimeNextReactToPlayer;                     // +0xa8
    bool WasAllowedToFollowPlayer;                  // +0xac
protected:                                          // `MAE` in the retail symbol
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_GameMaster::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    CAIStateGroup_GoToPub::InitStateGroup(world, creature);
    TimeNextReactToPlayer = 0;
    WasAllowedToFollowPlayer = false;
    PBaseThing = CIntelligentPointerThing(0);
}