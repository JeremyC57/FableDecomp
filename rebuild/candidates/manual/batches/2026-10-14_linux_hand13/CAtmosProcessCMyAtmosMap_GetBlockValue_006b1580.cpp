// CAtmosProcess::CMyAtmosMap::GetBlockValue(const C2DCoordI&) -- virtual slot 0: when the game object exists and
// the world's map is not in its disabled state, forwards the block coordinate to the map's lookup; otherwise 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B1580, length 50 bytes.
// Callees: CWorld::GetMap (vtable slot 12, +0x30) on the world at +0x24 of the global game object @ 0x013B86A0,
//          CWorldMap::GetBlockValue 0x0063A6E0 (tail-called: same argument, same `ret 4`).
// The map is fetched twice through the virtual call (not CSE'd) and the second one is a named local so the
// virtual call is evaluated before the argument is loaded (an inline expression pushes the argument first); the
// long result (`xor eax,eax`) and the pass-through argument make the forwarding a sibling-call `jmp`. The byte
// at +0x4c of the map is a disabled flag.
struct C2DCoordI;

struct CWorldMap {
    unsigned char _pad[0x4c];
    bool m_disabled;                                     // +0x4c
    long GetBlockValue(const C2DCoordI& block);          // 0x0063A6E0
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetMap();                         // slot 12 (+0x30)
};

struct CGame {
    unsigned char _pad[0x24];
    CWorld* m_pWorld;                                    // +0x24
};
extern CGame* g_pGame;                                   // 0x013B86A0

struct CMyAtmosMap {
    virtual long GetBlockValue(const C2DCoordI& block);
    virtual float GetBlockHeight(const C2DCoordI& block);
};

long CMyAtmosMap::GetBlockValue(const C2DCoordI& block)
{
    if (g_pGame) {
        if (!g_pGame->m_pWorld->GetMap()->m_disabled) {
            CWorldMap* map = g_pGame->m_pWorld->GetMap();
            return map->GetBlockValue(block);
        }
    }
    return 0;
}
