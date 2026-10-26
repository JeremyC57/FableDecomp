// CMeshBankStateBlock::GetDefault(CBankStateBlock* block) -- fill a state block with the default state: its 16-byte state at +4 cleared (the intrinsic memset: four zero dwords from one register), then the loaded flag (+4) set, the two bytes +5 / +6 cleared and the three dwords +8 / +0xc / +0x10 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23A30, length 47 bytes.
// Leaf: GetDefault
// Shape: `this` is not read, the block in eax, the state's address in esi for the constructor, a second zero register for the field stores, `ret 4`.
extern "C" void* __cdecl memset(void* block, int value, unsigned int size);
#pragma intrinsic(memset)
struct SBankState_00a23a30 {                       // 16 bytes at +4 of the block: zeroed by its inline constructor
    bool m_loaded;                                 // +0x04
    bool m_5;                                      // +0x05
    bool m_6;                                      // +0x06
    long m_8;                                      // +0x08
    long m_c;                                      // +0x0c
    long m_10;                                     // +0x10
};

struct CBankStateBlock_00a23a30 {
    virtual void v00();
    SBankState_00a23a30 m_state;                   // +0x04
};

class CMeshBankStateBlock { public:
    void GetDefault(CBankStateBlock_00a23a30* block);
};

void CMeshBankStateBlock::GetDefault(CBankStateBlock_00a23a30* block)
{
    memset(&block->m_state, 0, sizeof(block->m_state));
    block->m_state.m_loaded = true;
    block->m_state.m_5 = false;
    block->m_state.m_6 = false;
    block->m_state.m_8 = 0;
    block->m_state.m_c = 0;
    block->m_state.m_10 = 0;
}