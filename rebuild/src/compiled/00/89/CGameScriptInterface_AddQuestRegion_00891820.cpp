class CCharString;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CRegion;
class CWorldMap;

class CWorld
{
public:
    virtual void Slot00() {}
    virtual void Slot04() {}
    virtual void Slot08() {}
    virtual void Slot0C() {}
    virtual void Slot10() {}
    virtual void Slot14() {}
    virtual void Slot18() {}
    virtual void Slot1C() {}
    virtual void Slot20() {}
    virtual void Slot24() {}
    virtual void Slot28() {}
    virtual void Slot2C() {}
    virtual void Slot30() {}
    virtual CWorldMap* GetWorldMap() = 0;
};

class CWorldMap
{
public:
    long GetRegionNumberFromName(
        const CCharString& regionName) const;   // 0x004FC210
    const CRegion& GetRegion(long regionNumber) const;   // 0x004FC180
};

class CQuestManager
{
public:
    void AddQuestRegion(
        const CCharString& questName,
        const CRegion* region);
};

struct CGSIAddQuestRegion_Interface
{
    unsigned char m_Unused[0x04];
    CWorld* m_World;
};

class CGameScriptInterface
{
public:
    virtual void AddQuestRegion(
        const CCharString& questName,
        const CCharString& regionName) const;
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void CGameScriptInterface::AddQuestRegion(
    const CCharString& questName,
    const CCharString& regionName) const
{
    const long regionNumber =
        reinterpret_cast<const CGSIAddQuestRegion_Interface*>(
            this)->m_World->GetWorldMap()
            ->GetRegionNumberFromName(regionName);
    if (regionNumber > 0)
    {
        const CRegion& region =
            reinterpret_cast<const CGSIAddQuestRegion_Interface*>(
                this)->m_World->GetWorldMap()
                ->GetRegion(regionNumber);
        CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AddQuestRegion(
            questName,
            &region);
    }
}
