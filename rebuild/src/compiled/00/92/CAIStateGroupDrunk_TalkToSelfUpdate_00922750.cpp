// CAIStateGroup_Drunk::TalkToSelfUpdate() -- every so often a drunk mutters to himself: play the talk-to-self speech while idling, then schedule the next one 10..30 seconds later
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922750, 264 bytes (`ret`).
// Callees: the brain world accessor 0x008FCFC0 + the static frame counter 0x0049D870, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0xf), CTCTalk::IsTalking 0x006E28A0, CCharString(const char*, long) 0x0099EBF0
// ("TEXT_AI_OPINION_ACT_DRUNK_TALK_TO_SELF" 0x01290014) / ~CCharString 0x0099EAE0, CTCTalk::PlayAISpeech(const
// CCharString&, bool, EDialogueLayer, bool, bool) 0x006E3B30, AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40,
// GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, __ftol2 0x00BFEA70; 10.0f pool 0x01290004, fps global 0x01375550.
// Shape: the landed AI state-group model (0x00950A30); a `for (;;)` loop with a named string in its own block per speech
// attempt; `== 2` (still playing) idles with AIAction_DoNothing and `continue`s while it keeps going, else Terminate()
// and return; result 0 re-arms the timer with `frame + (rand(20) + 10) * fps` (fimul of the frame-rate global, VC copies
// it to the frame before the random call); a final `return` closes the loop body. That return stays its own epilogue that
// the re-arm store falls into -- the earlier `do { if (result != 2) { ...; return; } } while (...); Terminate();` form
// tail-merged the result != 0 exit into the Terminate epilogue and scheduled the store between the pops.
// Layout: Ego_r.pdb CAIStateGroup_Drunk FrameToNextTalkToSelf +0x14.
// Leaf: TalkToSelfUpdate@CAIStateGroup_Drunk
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
public:
    void Terminate();   // 0x008FCF40
};


class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

class CTCTalk { public:                                    // the interface-0xf component
    bool IsTalking();                               // 0x006E28A0
    long PlayAISpeech(const CCharString& text, bool allowNonPure, long layer, bool a, bool b);   // 0x006E3B30 (ESpeechPlaybackResult)
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
extern long g_framesPerSecond;                      // 0x01375550
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CAIStateGroup_Drunk : CAIStateGroupBase {
    long FrameToNextTalkToSelf;                     // +0x14
    unsigned long* GetPWorldSeed();                 // 0x008FCFD0
    long AIAction_DoNothing();                      // 0x0097E200 (EAIActionReturn)
    void TalkToSelfUpdate();
};

void CAIStateGroup_Drunk::TalkToSelfUpdate()
{
    if (((CWorld*)GetPWorld())->GetFrame() <= FrameToNextTalkToSelf)
        return;
    CTCTalk* talk;
    PBrain->PCreature->GetInterface(&talk, 0xf);
    if (talk->IsTalking())
        return;
    for (;;) {
        long result;
        {
            CCharString text("TEXT_AI_OPINION_ACT_DRUNK_TALK_TO_SELF", -1);
            result = talk->PlayAISpeech(text, false, 2, false, true);
        }
        if (result == 2) {
            if (AIAction_DoNothing() == 4)
                continue;
            Terminate();
            return;
        }
        if (result == 0) {
            CWorld* world = (CWorld*)GetPWorld();
            long framesPerSecond = g_framesPerSecond;
            FrameToNextTalkToSelf = (long)(world->GetFrame() +
                (GFFloatRandom(20.0f, GetPWorldSeed()) + 10.0f) * framesPerSecond);
        }
        return;
    }
}