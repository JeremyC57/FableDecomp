// CTCHeroStats::SetMorality(long) -- clamps and applies a morality change, with tutorial, GUI, event, sound and morph updates
// Leaf: SetMorality
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057C2E6, 523 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed SetAge 0x0057B05F).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC the CHeroStatsDef; def +0xD0 / +0xD4 the adult / child
//          morality limits), std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0, CPlayerGui::AddMoralityChange(long)
//          @ 0x00437930 (g_pPlayerGui 0x013B8790), CThingCreatureBase::IsChild() @ 0x00661F70, operator new @ 0x00BFEA1A,
//          CTCEntityEvents::AddEvent(CEventBase*) @ 0x008113C0, CTCHeroStats::CheckForNewExpressions() @ 0x0057BE71,
//          CCharString @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPos() @ 0x004C73D0,
//          CTCSoundPlayer::PlayUnattachedCriteria @ 0x0066BDF0, CTCHeroStats::GetMoralityPercentage() @ 0x0057A690,
//          CTCHeroMorph::SetMorality(float) @ 0x0071C000.
// Shape: the clamp is an inline by-reference limit over named bounds (min = -max; `v > hi` order); children re-clamp
// (written `change = Limit(); newMorality = change; change -= Morality;` for the direct edi load) against the
// child limit before the change is committed. The morality-changed event (vtable 0x0124F2DC) is built inline.
// Interfaces: 0x29 CTCHero (one-ternary pick: component straight into ecx), 0x42 CTCEntityEvents, 0x10 CTCSoundPlayer
// (`found = last; if (it != last && !(wanted < it->type)) found = it;`: retail keeps last in edi and overwrites it), 3 CTCHeroMorph.
// Layout: retail Morality +0x28 (PDB +0x30), MoralityChangingEnabled +0x2C (PDB +0x34).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    const C3DVector& GetPos() const;            // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfacePick(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceAlt(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = map->last;
            if (it != map->last && !(wanted < it->type))
                found = it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { public:
    bool IsChild();                                  // 0x00661F70
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

struct CEventBase {
    long WorldFrame;                                 // +0x04
    CEventBase() : WorldFrame(0) {}
    virtual ~CEventBase();
};

namespace NEntityEvents {
class CEventMoralityChanged : public CEventBase { public:          // vtable 0x0124F2DC
    long ChangedBy;                                  // +0x08
    CEventMoralityChanged(long changedBy) : ChangedBy(changedBy) {}
    virtual ~CEventMoralityChanged();                // 0x0057F36F
    virtual long GetType() const;                    // 0x0057A535
};
}

class CTCHero { public:
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                // 0x008113C0
};
class CTCSoundPlayer { public:
    unsigned long PlayUnattachedCriteria(const CCharString& name, const C3DVector& pos, bool a, bool b);  // 0x0066BDF0
};
class CTCHeroMorph { public:
    void SetMorality(float morality);                // 0x0071C000
};
class CPlayerGui { public:
    void AddMoralityChange(long change);             // 0x00437930
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0xD0];
    long          MaxMorality;                       // +0xD0
    long          MaxChildMorality;                  // +0xD4
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

template <class T> inline const T& LimitRef(const T& v, const T& lo, const T& hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCHeroStats : public CTCBase { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x20];
    long          Morality;                          // +0x28 (PDB +0x30)
    bool          MoralityChangingEnabled;           // +0x2C (PDB +0x34)

    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }
    void CheckForNewExpressions();                   // 0x0057BE71
    float GetMoralityPercentage();                   // 0x0057A690
    void SetMorality(long morality);
};

void CTCHeroStats::SetMorality(long morality)
{
    if (!MoralityChangingEnabled)
        return;
    long maxMorality = GetHeroStatsDef()->MaxMorality;
    long minMorality = -maxMorality;
    long newMorality = LimitRef(morality, minMorality, maxMorality);
    long change = newMorality - Morality;
    if (change != 0) {
        CTCHero* hero;
        if (ParentThing->GetInterfacePick(&hero, 0x29) && hero)
            hero->GiveTutorialForCategory(0x19, false);
        g_pPlayerGui->AddMoralityChange(change);
        if (((CThingCreatureBase*)ParentThing)->IsChild()) {
            long maxChild = GetHeroStatsDef()->MaxChildMorality;
            long minChild = -maxChild;
            change = LimitRef(morality, minChild, maxChild);
            newMorality = change;
            change -= Morality;
        }
        if (change != 0) {
            CTCEntityEvents* events;
            if (ParentThing->GetInterface(&events, 0x42))
                events->AddEvent(new NEntityEvents::CEventMoralityChanged(change));
            Morality = newMorality;
            CheckForNewExpressions();
            CTCSoundPlayer* sound;
            if (ParentThing->GetInterfaceAlt(&sound, 0x10))
                sound->PlayUnattachedCriteria("MORALITY_CHANGE", ParentThing->GetPos(), true, false);
        }
    }
    CTCHeroMorph* morph;
    if (ParentThing->GetInterface(&morph, 3))
        morph->SetMorality(GetMoralityPercentage());
}