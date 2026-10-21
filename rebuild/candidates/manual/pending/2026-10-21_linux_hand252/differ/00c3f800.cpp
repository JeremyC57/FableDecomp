// Grade: DIFFER(330v356) -- retail keeps a dead `count == 0 ? ::operator new(4) : ModuleAllocate(n*size+4)` split inside both new[] expressions (jne on the count>0 flags); a size test or a count-placement operator new[] folds it away. Flags: /EHsc, leaf InitializeActorArrays
// Leaf: InitializeActorArrays
// Flags: /EHsc
// CCombatActionManager_00c3f800::InitializeActorArrays() -- probe
void* __cdecl ModuleAllocate(unsigned int size);            // 0x00C3C360
extern long g_numPrimaryActors_0139c9b8;                    // 0x0139C9B8
extern long g_primaryActorsB_0139c9bc;                      // 0x0139C9BC
extern long g_numSecondaryActors_0139c9d0;                  // 0x0139C9D0
extern long g_secondaryActorsB_0139c9d4;                    // 0x0139C9D4

struct CPrimaryActor_00c3f800 {                             // 0x250 bytes
    unsigned char _body[0x250];
    CPrimaryActor_00c3f800();                               // 0x00C3F760
    ~CPrimaryActor_00c3f800();                              // 0x00C3F630
    static void* operator new[](unsigned int size, long count)
    {
        if (count)
            return ModuleAllocate(size);
        return ::operator new(size);
    }
    static void operator delete[](void* p, long count);
};
struct CSecondaryActor_00c3f800 {                           // 0x248 bytes
    unsigned char _body[0x248];
    CSecondaryActor_00c3f800();                             // 0x00C555E0
    ~CSecondaryActor_00c3f800();                            // 0x00C55130
    static void* operator new[](unsigned int size, long count)
    {
        if (count)
            return ModuleAllocate(size);
        return ::operator new(size);
    }
    static void operator delete[](void* p, long count);
};

struct CActorFreeList_00c3f800 {                            // 20 bytes
    unsigned char _body[0x14];
    void Init(void* block, long elementSize, long count);  // 0x00C3C8C0
};

struct CCombatActionManager_00c3f800 {
    unsigned char _pad[0x2c];
    CActorFreeList_00c3f800 m_primaryFree;                  // +0x2c
    CActorFreeList_00c3f800 m_secondaryFree;                // +0x40
    unsigned char _pad54[0x174 - 0x54];
    CPrimaryActor_00c3f800* m_pPrimaryActors;               // +0x174
    CSecondaryActor_00c3f800* m_pSecondaryActors;           // +0x178
    bool InitializeActorArrays();
};

bool CCombatActionManager_00c3f800::InitializeActorArrays()
{
    if (g_numPrimaryActors_0139c9b8 > 0) {
        m_pPrimaryActors = new (g_numPrimaryActors_0139c9b8) CPrimaryActor_00c3f800[g_numPrimaryActors_0139c9b8];
        if (!m_pPrimaryActors) {
            g_numPrimaryActors_0139c9b8 = 0;
            g_primaryActorsB_0139c9bc = 0;
            g_numSecondaryActors_0139c9d0 = 0;
            g_secondaryActorsB_0139c9d4 = 0;
            return false;
        }
        m_primaryFree.Init(m_pPrimaryActors, 0x250, g_numPrimaryActors_0139c9b8);
    }
    if (g_numSecondaryActors_0139c9d0 > 0) {
        m_pSecondaryActors = new (g_numSecondaryActors_0139c9d0) CSecondaryActor_00c3f800[g_numSecondaryActors_0139c9d0];
        if (!m_pSecondaryActors) {
            g_numPrimaryActors_0139c9b8 = 0;
            g_primaryActorsB_0139c9bc = 0;
            g_numSecondaryActors_0139c9d0 = 0;
            g_secondaryActorsB_0139c9d4 = 0;
            return false;
        }
        m_secondaryFree.Init(m_pSecondaryActors, 0x248, g_numSecondaryActors_0139c9d0);
    }
    return true;
}
