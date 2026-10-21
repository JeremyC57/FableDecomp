// CCombatSequenceGeneric::OnCreateSpecial(const map<CCharString, special-creator>& specials) -- a def with no action
// Grade: DIFFER(513v432) -- this in edi (retail esi) and the zero constant hoisted into ebx for the whole body, so each release compares against ebx after a reload instead of using the dec flags; the enum loop is not rotated like retail (pre-test + jmp into the body); calls, temporaries and the inlined push_back match
// list names a special sequence: create it through the registered creator, initialise it and copy its actions and
// action count; otherwise append one combat action per enum in the def's list; then mark the sequence created
// Leaf: ?OnCreateSpecial@CCombatSequenceGeneric
// Tried: one counted-pointer template (a), conditional assign + single-expression release (b), cached sequence pointer (c),
//   non-clearing sequence pointer (d, this).
// Layout: retail CCombatSequenceBase: NoAvailableActions +0x08, CombatActions +0x10 (STLport vector, 12 bytes),
//   Creature +0x1c, Target +0x20, CreateCalled +0x24; CCombatSequenceGeneric::Def +0x28 (PDB +0x2c, the debug vector is 16);
//   the def's action-enum vector at +0x54.
void __cdecl operator delete(void* block);            // 0x00BFE9BC
struct CThingCreatureBase;
struct CCombatSequenceBase;
struct CCharString {
    void* m_pData;
    ~CCharString();                                   // 0x0099EAE0
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template<class T> struct CCountedPointer {             // counted pointer {object, count block}
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    explicit CCountedPointer(T* object);              // out of line (0x006EF6F0 for CCombatSequenceBase)
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            Release();
            m_pObject = other.m_pObject;
            m_pCount = other.m_pCount;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
    void Release()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    ~CCountedPointer() { Release(); }
    T* operator->() const { return m_pObject; }
};
struct CSequencePointer {                             // CCountedPointer<CCombatSequenceBase>: release without clearing
    CCombatSequenceBase* m_pObject;
    SCountBlock*         m_pCount;
    explicit CSequencePointer(CCombatSequenceBase* object);   // 0x006EF6F0
    ~CSequencePointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
};
struct __false_type { };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCombatAction;
typedef CCountedPointer<CCombatAction> CCombatActionPointer;
struct CCombatActionVector {                          // STLport vector<CCountedPointer<CCombatAction>>
    CCombatActionPointer* _M_start;
    CCombatActionPointer* _M_finish;
    CCombatActionPointer* _M_end_of_storage;
    CCombatActionVector& operator=(const CCombatActionVector& other);   // 0x008B3180
    void _M_insert_overflow(CCombatActionPointer* position, const CCombatActionPointer& x, const __false_type&,
                            unsigned int fill_len, bool atend);   // 0x008B3350
    void push_back(const CCombatActionPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCombatActionPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
CCombatActionPointer __fastcall GetPCombatActionFromEnum(long action, CThingCreatureBase* creature, CThingCreatureBase* target);   // 0x00857430
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
struct CDefString { long ID; CDefString(); CDefString(const CDefString& o) : ID(o.ID) { } };
struct CCombatSequenceDef {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetSpecialName() const;       // slot 10
    unsigned char _pad04[0x54 - 4];
    CLongVector Actions;                              // +0x54
};
struct CTextTable {
    CCharString GetString(long id);                   // 0x009D49B0
};
extern CTextTable g_TextTable;                        // 0x013CA828
struct CCombatSequenceBase;
typedef CCombatSequenceBase* (__fastcall* SpecialCreator)(CThingCreatureBase& creature, CThingCreatureBase& target);
struct SSpecialNode {
    unsigned char  _pad[0x10];
    CCharString    Key;                               // +0x10
    SpecialCreator Creator;                           // +0x14
};
struct CSpecialMap {                                  // STLport map<CCharString, SpecialCreator>
    SSpecialNode* Header;
    long Count;
    SSpecialNode* Find(const CCharString& key) const; // 0x006F1360
};
struct CCombatSequenceBase {
    virtual void v00();
    virtual void Initialise();                        // slot 1
    bool                Running;                      // +0x04
    long                NoAvailableActions;           // +0x08
    long                CurrentActionIndex;           // +0x0c
    CCombatActionVector CombatActions;                // +0x10
    CThingCreatureBase* Creature;                     // +0x1c
    CThingCreatureBase* Target;                       // +0x20
    bool                CreateCalled;                 // +0x24
};
struct CCombatSequenceGeneric : CCombatSequenceBase {
    const CCombatSequenceDef* Def;                    // +0x28 CDefPointer<const CCombatSequenceDef>
    virtual void OnCreateSpecial(const CSpecialMap& specials);
};

void CCombatSequenceGeneric::OnCreateSpecial(const CSpecialMap& specials)
{
    if (Def->Actions._M_start == Def->Actions._M_finish) {
        CCharString name = g_TextTable.GetString(Def->GetSpecialName().ID);
        SSpecialNode* node = specials.Find(name);
        if (node != specials.Header) {
            CSequencePointer special(node->Creator(*Creature, *Target));
            CCombatSequenceBase* sequence = special.m_pObject;
            sequence->Initialise();
            CombatActions = sequence->CombatActions;
            NoAvailableActions = sequence->NoAvailableActions;
        }
    } else {
        for (long* it = Def->Actions._M_start; it != Def->Actions._M_finish; ++it) {
            CCombatActionPointer action;
            action = GetPCombatActionFromEnum(*it, Creature, Target);
            CombatActions.push_back(action);
            ++NoAvailableActions;
        }
    }
    CreateCalled = true;
}
