// CTCWillLightning::CLightningBeam::Deactivate(CIntelligentPointer<CThingCreatureBase>) -- stop an active beam (states 1/2/3): freeze its destination at the current end point and deactivate every child beam; a beam with children (unless already in state 2) waits for them (state 6 from 1, else 4); otherwise it starts deactivating (state 5) and plays the def's hit sound criteria on a live target, or at the end point through the live caster when the beam is not temporary
// Leaf: Deactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079FE90, 580 bytes (+ int3 padding), `ret 8` (by-value intelligent pointer, callee destroys).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (pointer
//   vtable 0x0123E7D8); itself (child beams); CVectorMap LowerBound 0x0040F020; CDefStringTable::GetString 0x009D49B0
//   (g_DefStringTable 0x013CA828); CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00 / PlayUnattachedCriteria 0x0066BDF0
//   (FableWin.pdb signatures); ~CCharString 0x0099EAE0; operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCWillLightning::CLightningBeam up to the child list (+0x4c); the retail STLport list is 4 bytes (PDB 12),
//   so the later members sit 8 lower (m_bTemporary +0x50, PDef +0x74); the def's +0x11c CDefString is the sound name
//   (descriptive field name). Model shared with the landed CTCWillLightning::Deactivate 0x007A0310 and
//   CLightningBeam::IsTargetValid 0x0079FD90.
// Shape: the list size() is the counting loop (`size() != 0`); the interface lookup uses the one-ternary entry pick.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
};
class CDefString { public: long Index; };
struct CDefStringTable {
    CCharString GetString(CDefString name);           // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body): only its visibility matters.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
    unsigned long PlayUnattachedCriteria(const CCharString& name, const C3DVector& pos, bool a, bool b);   // 0x0066BDF0
};
class CCPPointerInfo { public:                          // counted-pointer info block
    long Count;                                  // +0
    void (__fastcall* Destroy)(void*);           // +4 per-type deleter, object in ecx
    void* Object;                                // +8
};
void operator delete(void* p);                   // 0x00BFE9BC
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
template <class T> struct CRetailList {          // STLport list: the sentinel node pointer only (4 bytes; PDB 12)
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
    Node* begin() const { return Head->Next; }
    Node* end() const { return Head; }
    unsigned long size() const { unsigned long n = 0; for (Node* it = Head->Next; it != Head; it = it->Next) ++n; return n; }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                               // +4
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                    // 0x00A01B50
    void SetPItem(CThing* item);                 // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(((CThing*)other.GetPItem())); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CSpecialAbilitiesLightningSpellDef { public:
    unsigned char _pad[0x11c];
    CDefString    HitSound;                      // +0x11c (descriptive name)
};
struct CLightningBeam {
    long                    m_lsState;           // +0x00 ELightningState
    CBaseIntelligentPointer m_tpParticleHandle;  // +0x04
    CBaseIntelligentPointer m_tcpHome;           // +0x0c
    CBaseIntelligentPointer m_tcpTarget;         // +0x14
    C3DVector               m_vStartPos;         // +0x1c
    C3DVector               m_vEndPos;           // +0x28
    C3DVector               m_vDestination;      // +0x34
    float                   m_fDamagePerHit;     // +0x40
    float                   m_fTime;             // +0x44
    float                   m_fTimeBase;         // +0x48
    CRetailList<CCountedPointer<CLightningBeam> > m_lChildren;   // +0x4c
    bool                    m_bTemporary;        // +0x50
    bool                    m_bMainBeam;         // +0x51
    bool                    m_bRightHand;        // +0x52
    float                   m_fActivationTime;   // +0x54
    float                   m_fDeActivationTime; // +0x58
    float                   m_fLife;             // +0x5c
    float                   m_fDecapitationTime; // +0x60
    float                   m_fDecapitationClock;   // +0x64
    float                   m_fDelay;            // +0x68
    long                    m_OnhitEveryFrame;   // +0x6c
    long                    m_nFrameCount;       // +0x70
    CSpecialAbilitiesLightningSpellDef* PDef;    // +0x74 CDefPointer
    void Deactivate(CIntelligentPointer player);
};

void CLightningBeam::Deactivate(CIntelligentPointer player)
{
    if (m_lsState == 3 || m_lsState == 2 || m_lsState == 1) {
        m_vDestination = m_vEndPos;
        for (CRetailList<CCountedPointer<CLightningBeam> >::Node* it = m_lChildren.begin(); it != m_lChildren.end(); it = it->Next) {
            CCountedPointer<CLightningBeam> child = it->Value;
            child->Deactivate(player);
        }
        if (m_lsState != 2 && m_lChildren.size() != 0) {
            if (m_lsState == 1) {
                m_fTimeBase = m_fTime;
                m_lsState = 6;
            } else {
                m_fTimeBase = m_fTime;
                m_lsState = 4;
            }
            return;
        }
        m_fTimeBase = m_fTime;
        m_lsState = 5;
        if (IS_THING_ALIVE(m_tcpTarget)) {
            CTCSoundPlayer* sound;
            if (((CThing*)m_tcpTarget.GetPItem())->GetInterface(&sound, 0x10)) {
                CCharString name = g_DefStringTable.GetString(PDef->HitSound);
                sound->PlayAttachedCriteria(name, true, false, false, -1);
            }
        } else if (IS_THING_ALIVE(player) && !m_bTemporary) {
            CTCSoundPlayer* sound;
            if (((CThing*)player.GetPItem())->GetInterface(&sound, 0x10)) {
                CCharString name = g_DefStringTable.GetString(PDef->HitSound);
                sound->PlayUnattachedCriteria(name, m_vEndPos, false, false);
            }
        }
    }
}