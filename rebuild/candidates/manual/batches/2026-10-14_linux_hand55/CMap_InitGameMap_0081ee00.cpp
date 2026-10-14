// CMap::InitGameMap() -- build a new CGameMap (0x24 bytes, operator new @ 0x00BFEA1A, constructor @ 0x006B7760 with (the world +0x20, this, a copy of the 8-byte init record at +0x1d1c)) into +0x1a78, deleting the previous one through its virtual destructor (slot 0 with 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081EE00, length 88 bytes.
// Callees: operator new @ 0x00BFEA1A, CGameMap::CGameMap(CWorld*, CMap*, CGameMapInit*) @ 0x006B7760, the old map's virtual slot 0 (the deleting destructor with 1). Shape: the init record copied to the frame before `new`, the null arm (`xor eax,eax`) joined before the store, the old pointer read after the constructor (a local taken before the store) and deleted after it, `ret`.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
struct CWorld;
struct CMap;
struct CGameMapInit { long m_a; long m_b; };       // the 8-byte record at +0x1d1c
struct CGameMap {                                  // 0x24 bytes
    virtual ~CGameMap();                           // slot 0
    unsigned char _body[0x20];
    CGameMap(CWorld* world, CMap* map, CGameMapInit* init);   // 0x006B7760
};
struct CMap {
    unsigned char _pad[0x20];
    CWorld*       m_pWorld;                        // +0x20
    unsigned char _pad24[0x1a54];
    CGameMap*     m_pGameMap;                      // +0x1a78
    unsigned char _pad1a7c[0x2a0];
    CGameMapInit  m_gameMapInit;                   // +0x1d1c
    void InitGameMap();
};

void CMap::InitGameMap()
{
    CGameMapInit init = m_gameMapInit;
    CGameMap* map = new CGameMap(m_pWorld, this, &init);
    CGameMap* old = m_pGameMap;
    m_pGameMap = map;
    if (old)
        delete old;
}
