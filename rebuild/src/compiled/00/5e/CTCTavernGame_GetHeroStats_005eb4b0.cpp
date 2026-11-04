// CTCTavernGame::GetHeroStats(CTCHeroStats** out) -- hands out the living hero creature's hero-stats component (interface 4), leaving *out untouched otherwise
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pWorld (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EB4B0, length 92 bytes (ret 4). Name and signature: Ego_r.pdb
// `?GetHeroStats@CTCTavernGame@@QAEXPAPAVCTCHeroStats@@@Z`.
// Callees: CPlayerManager::GetMainPlayer() @ 0x00449960 and CPlayer::GetPControlledCreature() @ 0x00487DC0 (landed naming,
// 0x007B1EB0), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (another out-of-line copy of the interface
// map search). The player manager is the member at +0x1c of the global world object @ 0x013B86A0.
// Layout: the hero's dead bit is bit 0 of +0x91 (landed 0x00836BE0 / 0x007B1EB0); the interface-present bitset is at +0x20
// (interface 4 = `test byte ptr [eax+20h],10h`), the interface map at +0x44.
// Shape: the inline out-parameter GetInterface(T** ppOut, long type) template of the landed 0x007504E0 / 0x007B2CF0 family
// writes straight through the caller's pointer; the key 4 lives in the `push ecx` frame slot. Here the template must be
// `__forceinline`: with its bool result unused VC7.1 keeps it out of line (50v92).
class CTCBase;
class CTCHeroStats;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    unsigned char _pad_0x54[0x3d];
    unsigned char m_flags;                   // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;             // 0x00487DC0
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;                // 0x00449960
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PPlayerManager;          // +0x1c
};

class CTCTavernGame { public:
    void GetHeroStats(CTCHeroStats** out);
};

void CTCTavernGame::GetHeroStats(CTCHeroStats** out)
{
    CThing* hero = CMainGameComponent::Get()->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
        hero->GetInterface(out, 4);
}