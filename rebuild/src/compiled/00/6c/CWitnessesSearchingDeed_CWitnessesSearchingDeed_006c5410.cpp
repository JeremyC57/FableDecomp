// CWitnessesSearchingDeed::CWitnessesSearchingDeed(long handle, EOpinionDeedType type, CThingAICreature* recipient, EOpinionPostDeedType postType, CFaction* faction, bool reverseFactionSearch) -- an opinion deed waiting for witnesses: handle, deed type, the recipient (intelligent pointer), post type, faction and the reverse-faction-search flag
// Leaf: ??0CWitnessesSearchingDeed@@QAE@JW4EOpinionDeedType@@PAVCThingAICreature@@W4EOpinionPostDeedType@@PAVCFaction@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5410, 73 bytes (+7 int3 padding), thiscall, `ret 18h`.
// Layout: Ego_r.pdb CWitnessesSearchingDeed (28 bytes): DeedHandle +0x00, DeedType +0x04, Recipient
//   (CIntelligentPointer<CThingAICreature>, 8 bytes) +0x08, PostType +0x10, PFaction +0x14, ReverseFactionSearch +0x18.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90 (names as the landed
//   CThingSoundEmitter::~CThingSoundEmitter 0x00836A50); CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0.
// Shape: init list; the pointer member's inline ctor runs the base ctor out of line, stores its own vtable, then
//   SetPItem(recipient). No EH frame (the deed is built in a TU without /EHsc).
class CThing { public: virtual ~CThing(); };
class CThingAICreature : public CThing { };
class CFaction;
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NONE };

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    void SetPItem(CThing* item);                    // 0x00A01B90
    CThing* PItem;                                  // +0x04
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
};

class CWitnessesSearchingDeed {
public:
    CWitnessesSearchingDeed(long handle, EOpinionDeedType type, CThingAICreature* recipient,
                            EOpinionPostDeedType postType, CFaction* faction, bool reverseFactionSearch);
    long                                  DeedHandle;             // +0x00
    EOpinionDeedType                      DeedType;               // +0x04
    CIntelligentPointer<CThingAICreature> Recipient;              // +0x08
    EOpinionPostDeedType                  PostType;               // +0x10
    CFaction*                             PFaction;               // +0x14
    bool                                  ReverseFactionSearch;   // +0x18
};

CWitnessesSearchingDeed::CWitnessesSearchingDeed(long handle, EOpinionDeedType type, CThingAICreature* recipient,
                                                 EOpinionPostDeedType postType, CFaction* faction,
                                                 bool reverseFactionSearch)
    : DeedHandle(handle), DeedType(type), Recipient(recipient), PostType(postType), PFaction(faction),
      ReverseFactionSearch(reverseFactionSearch)
{
}