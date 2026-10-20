// CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn() -- once the wedding fresco has faded in, the wife's def sub-def (CWifeDef) is fetched from her thing's def and a positive Dowry is paid into the living hero's CTCHeroStats
// Grade: DIFFER(ours v retail) -- 175v163: our this copied to ebx + lea edi,[ecx+28h] def CSE and 8-byte frame vs retail mov edi,[esp+8] 4-byte frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF480, 163 bytes (+13 int3). __fastcall, `ret`.
// Callees: CDefClassBase::GetSubDef<CWifeDef>(CDefPointer<const CWifeDef>*) 0x006D3DC0 (manifest label names the
//          CAICreatureWillPowerIndicatorDef instantiation; propagated), CTCBase::GetPlayerManager() 0x00686D40,
//          CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetHeroThing() 0x00487DC0, LowerBound 0x0040F020,
//          CTCHeroStats::AddMoney(long, bool) 0x0057B327, the sub-def's slot 1 on release.
// Shape: the owner's def pointer (CThing PDef +0x70) is read into a local and null-tested first; the out pointer is
//        zeroed between that load and its test (`mov [esp+4],0`); on the found path the sub-def is read back from the out
//        slot into edi right after the call (before `test al,al`), the Dowry test guards the hero lookup, interface 4
//        (CTCHeroStats, byte +0x20 bit 4) is fetched through the out-parameter form into the shared key word and AddMoney
//        is called with the def's Dowry (re-read) and true; the def is released by the holder's destructor at the end (null
//        test, `dec [edi+4]`, slot 1). The early return on a null owner def skips the release because the sub-def is known null.
// Layout: CTCWife ParentThing +0x04; CThing PDef +0x70; CWifeDef Dowry +0x28 (PDB 40).
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
};
struct CWifeDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Dowry;                                 // +0x28
};
// The sub-def taken out of the out pointer right after GetSubDef and released by the holder's destructor: retail keeps
// the def in one register (edi) for the Dowry reads and the release; a plain local splits the Dowry address off.
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CWifeDef>* out); // 0x006D3DC0
};
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
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
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CTCHeroStats {
    void AddMoney(long amount, bool showMessage);   // 0x0057B327
};
struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    CPlayerManager* GetPlayerManager();          // 0x00686D40 (CTCBase)
    void InitWifeAtEndOfMarriageFrescoEndFadeIn();
};
void CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn()
{
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CWifeDef> wifeDef;
    wifeDef.Object = 0;
    if (thingDef) {
        bool found = thingDef->GetSubDef(&wifeDef);
        CDefHolder<CWifeDef> def(wifeDef.Object);
        if (found && def->Dowry > 0) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (hero && !(hero->Flags91 & 1)) {
                CTCHeroStats* stats;
                if (hero->GetInterface(&stats, 4))
                    stats->AddMoney(def->Dowry, true);
            }
        }
    }
}
