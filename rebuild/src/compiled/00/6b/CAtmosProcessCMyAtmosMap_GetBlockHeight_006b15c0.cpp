// CAtmosProcess::CMyAtmosMap::GetBlockHeight(const C2DCoordI&) -- virtual slot 1: when the game object exists and
// the world's map is not in its disabled state, returns the map's ground height at the block; otherwise 0.0f.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B15C0, length 54 bytes.
// Callees: CWorld::GetMap (vtable slot 12, +0x30) on the world at +0x24 of the global game object @ 0x013B86A0,
//          CWorldMap::GetGroundSizeZAtBlock 0x0063A660 (float). The 0.0f comes from the constant pool @ 0x0122DEDC.
// The map is a named local (the virtual call is evaluated before the argument is loaded) and the float result
// is kept in a named local, which is what stops the forwarding from becoming a sibling-call `jmp` (slot 0 at
// 0x006B1580, a long result, does tail-call).
class C2DCoordI;

class CWorldMap { public:
    unsigned char _pad[0x4c];
    bool m_disabled;                                     // +0x4c
public:
    float GetGroundSizeZAtBlock(const C2DCoordI&) const;   // 0x0063A660
};

class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetMap();                         // slot 12 (+0x30)
};

class CGame { public:
    unsigned char _pad[0x24];
    CWorld* m_pWorld;                                    // +0x24
};
extern CGame* g_pGame;                                   // 0x013B86A0

struct CMyAtmosMap {
    virtual long GetBlockValue(const C2DCoordI& block);
    virtual float GetBlockHeight(const C2DCoordI& block);
};

float CMyAtmosMap::GetBlockHeight(const C2DCoordI& block)
{
    if (g_pGame) {
        if (!g_pGame->m_pWorld->GetMap()->m_disabled) {
            CWorldMap* map = g_pGame->m_pWorld->GetMap();
            float height = map->GetGroundSizeZAtBlock(block);
            return height;
        }
    }
    return 0.0f;
}