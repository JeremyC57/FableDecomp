// Leaf: ?SetCreatureGeneratorsCreatureGroupAsEnabled@CGameScriptInterface@@UBEXW4ECreatureGroup@@_N@Z
enum ECreatureGroup { ECreatureGroup_Dummy };

class CWorld
{
public:
    void SetCreatureGenerationGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled);
};

class CGameScriptInterface
{
public:
    virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled) const;
};

struct CGameScriptInterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CWorld* m_pWorld; /* 0x04 */
};

void CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled(
    ECreatureGroup creatureGroup,
    bool enabled) const
{
    reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_pWorld
        ->SetCreatureGenerationGroupAsEnabled(creatureGroup, enabled);
}