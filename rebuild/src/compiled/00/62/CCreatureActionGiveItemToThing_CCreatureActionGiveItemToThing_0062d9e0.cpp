// CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingGiven) -- a creature action playing a random STANDARD_GIVE_OBJECT animation (base: no time limit, 0.75 blend, zeroed sound pair), holding intelligent pointers to the receiver and the item
// Leaf: ??0CCreatureAction_GiveItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062D9E0, 161 bytes, thiscall, `ret 0Ch`; vtable 0x0125919C
//   (.?AVCCreatureAction_GiveItemToThing@@).
// Identity (proven): Ego_r.pdb / FableWin.pdb ??0CCreatureAction_GiveItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z
//   (three references, `ret 0Ch`), storing that class's RTTI vtable; Ego_r.pdb members PReceivingThing / PItemBeingGiven
//   (CIntelligentPointer<CThing>, the last two members, here at +0xA8 / +0xB0 after the 0xA8-byte retail base).
// Callees: CCharString(const char*, long) 0x0099EBF0 ("STANDARD_GIVE_OBJECT" 0x012592B4, -1), CCreatureActionBase::
//   GetRandomAnimFromName(const CThingCreatureBase&, const CCharString&) 0x006924B0 (protected static __fastcall: it pushes
//   1 and calls the creature's own GetRandomAnimFromName), CCreatureActionBase(CThingCreatureBase&, float, float, const
//   CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30, ~CCharString 0x0099EAE0, CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90 (inline CIntelligentPointer<CThing>(CThing*): base ctor,
//   vtable 0x01238C6C, SetPItem).
// Label correction: catalogued as "CCreatureAction_GiveItemToThing" with a gap-scan module; the landed
//   PickUpThingAtTable ctor 0x008440C0 binds 0x006924B0 as GetTableAnimation(CThingCreatureBase*, long) (NAME_FIXES.tsv).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CThing;
class CThingCreatureBase;
class CAnimationEntry;
class CSoundPair {
public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCreatureActionBase {                                 // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase& creature, float totalTime, float blendTime, const CAnimationEntry& anim,
                        const CSoundPair& sounds, long interruptionGroupID, long interruptionPriority);   // 0x00693B30
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                   const CCharString& name);   // 0x006924B0
};
class CCreatureAction_GiveItemToThing : public CCreatureActionBase {   // vtable 0x0125919C
public:
    CCreatureAction_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingGiven);
    CIntelligentPointer<CThing> PReceivingThing;            // +0xA8
    CIntelligentPointer<CThing> PItemBeingGiven;            // +0xB0
};

CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing,
                                                                 CThing& itemBeingGiven)
    : CCreatureActionBase(creature, -1.0f, 0.75f, GetRandomAnimFromName(creature, "STANDARD_GIVE_OBJECT"),
                          CSoundPair(), 0, 0)
    , PReceivingThing(&receivingThing)
    , PItemBeingGiven(&itemBeingGiven)
{
}