// CCombatAction_FallStunned::Initialise() -- push a fresh CModeDazed (not yet started) onto the creature's mode manager, then the base CCombatActionBase::Initialise
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED950, 192 bytes incl. padding (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, twice), CVectorMap::LowerBound @ 0x0040F020
//          (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24; the result is not tested, so a missing
//          manager leaves the slot unwritten), operator new @ 0x00BFEA1A (0x84-byte CModeDazed, 12-byte count block),
//          CModeDazed::CModeDazed(CThing*) @ 0x00740160, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>)
//          @ 0x0073ACC0, CCombatActionBase::Initialise @ 0x00857520; count-block destroy fn 0x007180B0.
// Layout: retail CCombatActionBase model (PCreature +0x08); CModeDazed +0x6c a started flag.
// Shape: the by-value counted pointer is built in the argument area by the inline CCountedPointer(T*) ctor of the
//          landed 0x00718A00 (a user copy ctor makes the argument build in place); its null test folds away because the new mode was already dereferenced (`[eax+6Ch]`).
void __fastcall Destroy_007180b0(void* object);   // 0x007180B0

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};

struct CModeBase { };
struct CModeDazed : CModeBase {
    unsigned char _pad[0x6c];
    bool          Started;                      // +0x6c
    unsigned char _pad6d[0x84 - 0x6d];
    CModeDazed(CThing* creature);               // 0x00740160
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
};

class CCombatAction_FallStunned : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_FallStunned::Initialise()
{
    CTCCreatureModeManager* modeManager;
    PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
    CModeDazed* mode = new CModeDazed(PCreature.GetPItem());
    mode->Started = false;
    modeManager->AddMode(CCountedPointer<CModeBase>(mode));
    CCombatActionBase::Initialise();
}