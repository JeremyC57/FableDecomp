// NUISystem::CMenuEntry::InitializeRumbleActions() -- copy the def's rumble repeat setting, then for each non-zero rumble id in the def (selected, back, pre, visible, unselected) create a CAction carrying the id and add it to the matching action list
// Leaf: ?InitializeRumbleActions@CMenuEntry@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053DA30, length 672 bytes (+ int3 padding), `ret`.
// Callees: own virtual slot 108 (+0x1b0, GetDef returning a CDefPointer by value), operator new 0x00BFEA1A,
//   NUISystem::CAction::CAction 0x0042BE50 (landed), CCountedPointer<CAction>(CAction*) 0x0042AA29, own virtual slots 66
//   (+0x108), 68 (+0x110), 67 (+0x10c), 152 (+0x260) and 69 (+0x114) taking `const CCountedPointer<CAction>&`,
//   operator delete 0x00BFE9BC (the count block).
// Layout: def +0x184 copied to this +0x18c; the five rumble ids at def +0xc4/+0xc8/+0xcc/+0xd0/+0xd8; the def is
//   ref-counted (RefCount +4, slot 1 destroys); the counted pointer's count block is {count, __fastcall deleter, object}.
// Shape: the counted-pointer and def-pointer destructors are inline (the def release through a CRefCounted base at +4,
//   DecRef then GetRefCount, which gives retail's `add eax,4 ; dec ; mov edx,esi ; test edx,edx`); the CAction is
//   reached through operator->. operator new is the global 0x00BFEA1A.
void operator delete(void* p);                   // 0x00BFE9BC

struct SCountBlock {
    long  m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);      // +4
    void* m_pObject;                             // +8
};
template <class T> struct CCountedPointer {
    T*           m_pObject;                      // +0
    SCountBlock* m_pCount;                       // +4
    CCountedPointer(T* object);                  // 0x0042AA29
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    T* operator->() const { return m_pObject; }
};

struct CRefCounted {                             // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CMenuEntryDef {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    CRefCounted   Ref;                           // +0x04
    unsigned char _pad08[0xc4 - 8];
    long          RumbleOnSelected;              // +0xc4
    long          RumbleOnBack;                  // +0xc8
    long          RumblePre;                     // +0xcc
    long          RumbleOnVisible;               // +0xd0
    long          _d4;
    long          RumbleOnUnselected;            // +0xd8
    unsigned char _padDC[0x184 - 0xdc];
    long          RumbleRepeat;                  // +0x184
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Data;
    ~CDefPointer()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
    T* operator->() const { return Data; }
};

namespace NUISystem {
struct CAction {                                 // retail 0x10 bytes
    long  ActionType;                            // +0x00
    void* UIActionParams;                        // +0x04
    void* ThingActionParams;                     // +0x08
    bool  Valid;                                 // +0x0C
    CAction();                                   // 0x0042BE50
};

struct CMenuEntry {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065();
    virtual void AddActionOnSelected(const CCountedPointer<CAction>& action);     // slot 66 (+0x108)
    virtual void AddPreAction(const CCountedPointer<CAction>& action);           // slot 67 (+0x10c)
    virtual void AddActionOnBack(const CCountedPointer<CAction>& action);        // slot 68 (+0x110)
    virtual void AddActionOnUnselected(const CCountedPointer<CAction>& action);  // slot 69 (+0x114)
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<CMenuEntryDef> GetDef();                                  // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151();
    virtual void AddActionOnVisible(const CCountedPointer<CAction>& action);     // slot 152 (+0x260)
    unsigned char _body[0x18c - 4];
    long          RumbleRepeat;                                                   // +0x18c
    void InitializeRumbleActions();
};

void CMenuEntry::InitializeRumbleActions()
{
    CDefPointer<CMenuEntryDef> def = GetDef();
    RumbleRepeat = def->RumbleRepeat;
    if (def->RumbleOnSelected) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->RumbleOnSelected;
        AddActionOnSelected(action);
    }
    if (def->RumbleOnBack) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->RumbleOnBack;
        AddActionOnBack(action);
    }
    if (def->RumblePre) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->RumblePre;
        AddPreAction(action);
    }
    if (def->RumbleOnVisible) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->RumbleOnVisible;
        AddActionOnVisible(action);
    }
    if (def->RumbleOnUnselected) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->RumbleOnUnselected;
        AddActionOnUnselected(action);
    }
}

}
