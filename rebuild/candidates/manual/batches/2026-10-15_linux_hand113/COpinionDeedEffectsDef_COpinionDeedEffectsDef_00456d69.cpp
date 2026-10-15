// COpinionDeedEffectsDef::COpinionDeedEffectsDef() -- base CParentDefClassBase(), own vtable, the Effects list built with a default allocator, the Reaction and WarningText vectors empty
// Leaf: ??0COpinionDeedEffectsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00456D69, length 53 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370, the STLport list constructor
// list(const allocator_type&) @ 0x00450D90 (zeroes the node word, mallocs the 0x20-byte sentinel and links
// it to itself; manifest "LinkedList_Initialize"). Vtable 0x01233E54 = COpinionDeedEffectsDef.
// Label: the catalog name "CUILocaleGraphicsDef::CUILocaleGraphicsDef" is propagated; the vtable stored is
// COpinionDeedEffectsDef's (rtti-vtables.tsv slot 0 = 0x00456DA4, the sibling right after this body).
// Layout: PDB COpinionDeedEffectsDef (148 bytes) sits 4 bytes high from the list on (STLport list = one
// node word in retail): the four timing fields +0x3c..+0x48, Effects (list) +0x4c, ReactionPriority +0x50,
// Reaction (vector) +0x54, ReactionDialogueLayer +0x60, WarningText (vector) +0x64, CrimeSeverity +0x70,
// CrimeFineAmount +0x74, Tutorial +0x78, the two flag bits +0x7c.
// Shape: the list's default allocator argument is an empty temporary homed in the high byte of the
// `push ecx` frame slot (`lea eax,[esp+7]`); the STLport vectors' inline constructors zero their three
// words from one `xor eax,eax` after the list (members with inline constructors follow the vtable store).
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                     // +0x04 .. +0x3c
    CParentDefClassBase();                         // 0x00430370
};

template <class T> struct allocator {
    allocator() { }
};

struct COpinionTransientOffset;
struct COpinionDeedReactionDef;
struct CCharString;

template <class T> struct list {                   // STLport list: one node word
    void* _M_node;
    list(const allocator<T>& a = allocator<T>());  // 0x00450D90
};

template <class T> struct vector {                 // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct COpinionDeedEffectsDef : CParentDefClassBase {   // vtable 0x01233E54
    long                              KnowledgeLifeInSeconds;                     // +0x3c
    long                              MinimumSecondsBetweenPostings;              // +0x40
    long                              MinimumSecondsBetweenEffectsForIndividual;  // +0x44
    float                             PreReactionDelayIgnoreOnDelay;              // +0x48
    list<COpinionTransientOffset>     Effects;                                    // +0x4c
    long                              ReactionPriority;                           // +0x50
    vector<COpinionDeedReactionDef>   Reaction;                                   // +0x54
    long                              ReactionDialogueLayer;                      // +0x60 EDialogueLayer
    vector<CCharString>               WarningText;                                // +0x64
    long                              CrimeSeverity;                              // +0x70 ECrimeSeverity
    long                              CrimeFineAmount;                            // +0x74
    long                              Tutorial;                                   // +0x78 ETutorialCategory
    unsigned char                     CanBeHeard : 1;                             // +0x7c
    unsigned char                     ReactionRestrictToTarget : 1;
    virtual void v00();
    COpinionDeedEffectsDef();
};

COpinionDeedEffectsDef::COpinionDeedEffectsDef()
{
}
