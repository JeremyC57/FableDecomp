// CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn() -- once the wedding fresco has faded in, the wife's def sub-def (CWifeDef) is fetched from her thing's def and a positive Dowry is paid into the living hero's CTCHeroStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF480, 163 bytes (+13 int3). __fastcall, `ret`.
// Callees: CDefClassBase::GetSubDef<CWifeDef>(CDefPointer<const CWifeDef>*) 0x006D3DC0 (manifest label names the
//          CAICreatureWillPowerIndicatorDef instantiation; propagated), CTCBase::GetPlayerManager() 0x00686D40,
//          CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetHeroThing() 0x00487DC0, LowerBound 0x0040F020,
//          CTCHeroStats::AddMoney(long, bool) 0x0057B327, the sub-def's slot 1 on release.
// Shape: the owner's def pointer (CThing PDef +0x70) is read into a local and null-tested first; the sub-def pointer is
//        zeroed between that load and its test (`mov [esp+4],0`); GetSubDef is a same-TU noinline stand-in body that fills
//        it through `T**` (writes only the out slot), so it is read back into edi right after the call (before `test al,al`)
//        and kept there for the Dowry reads and the hand release; the Dowry test guards the hero lookup; interface 4
//        (CTCHeroStats, byte +0x20 bit 4) is fetched through the out-parameter GetInterface whose end/hit choice is one
//        ternary (the component lands straight in ecx) into the shared key word; AddMoney gets the re-read Dowry and true.
//        The early return on a null owner def skips the release because the sub-def is known null.
// Layout: CTCWife ParentThing +0x04; CThing PDef +0x70; CWifeDef Dowry +0x28 (PDB 40).
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
};
class CWifeDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long Dowry;                                 // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CWifeDef** out); // 0x006D3DC0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWifeDef** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (CWifeDef*)def;
    return true;
}
#pragma auto_inline(on)
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetHeroThing() const;               // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CTCHeroStats { public:
    void AddMoney(long amount, bool showMessage);   // 0x0057B327
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCWife : public CTCBase { public:
    CThing* ParentThing;                         // +0x04
    void InitWifeAtEndOfMarriageFrescoEndFadeIn();
};
void CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn()
{
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CWifeDef* def = 0;
    if (thingDef) {
        if (thingDef->GetSubDef(&def) && def->Dowry > 0) {
            CThing* hero = GetPlayerManager().GetMainPlayer().GetHeroThing();
            if (hero && !(hero->Flags91 & 1)) {
                CTCHeroStats* stats;
                if (hero->GetInterface(&stats, 4))
                    stats->AddMoney(def->Dowry, true);
            }
        }
        if (def && --def->m_refCount == 0)
            def->Destroy();
    }
}