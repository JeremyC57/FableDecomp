// CWitnessesSearchingDeed::CWitnessesSearchingDeed(const CWitnessesSearchingDeed&) -- copy constructor of the 0x1C-byte opinion "witnesses searching" deed record (re-registers the recipient intelligent pointer)
// Leaf: ??0CWitnessesSearchingDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006c5460, 77 bytes (`ret 4`, returns this).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Identity: the catalog label `_Cons_val<allocator<CWitnessesSearchingDeed>,..>` is the construct
// helper that calls it; this body is a thiscall copy constructor (this in ecx, source on the stack,
// `mov eax,esi` return).  Callers 0x006C78A5.. are the deed vector's insert/copy paths.
// Layout: Ego_r.pdb CWitnessesSearchingDeed (0x1C) = long DeedHandle +0, EOpinionDeedType DeedType
// +4, CIntelligentPointer<CThingAICreature> Recipient +8 (vtable 0x0123E7E0, as the landed
// CTCBoss::SetHelperCreature 0x007AF310 notes), EOpinionPostDeedType PostType +0x10,
// CFaction* PFaction +0x14, bool ReverseFactionSearch +0x18.
// Shape: the base ctor + vtable store come BEFORE the two leading dword copies, and the pointer is
// then re-pointed with Get/SetPItem: Recipient is default-constructed and the body is `*this = other`
// through the implicit memberwise operator= (memberwise mem-initializers, or an implicit copy
// constructor, copy +0/+4 first and then run the pointer's whole copy constructor).
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;
class CThingAICreature;
class CBaseObject;
class CFaction;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThingAICreature>: vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NONE };

class CWitnessesSearchingDeed { public:
    long                                  DeedHandle;            // +0x00
    EOpinionDeedType                      DeedType;              // +0x04
    CIntelligentPointer<CThingAICreature> Recipient;             // +0x08
    EOpinionPostDeedType                  PostType;              // +0x10
    CFaction*                             PFaction;              // +0x14
    bool                                  ReverseFactionSearch;  // +0x18
    CWitnessesSearchingDeed(const CWitnessesSearchingDeed& other);
};

CWitnessesSearchingDeed::CWitnessesSearchingDeed(const CWitnessesSearchingDeed& other)
{
    *this = other;
}