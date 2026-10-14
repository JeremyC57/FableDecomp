#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0, g_blocks = 0;
struct CStateBlocksBase_00b59710 { virtual void v00() { } unsigned char _pad[8]; CStateBlocksBase_00b59710() { ++g_base; } };
struct CEngineStateBlock { virtual void v00() { } CEngineStateBlock() { ++g_blocks; } };
struct CEngineStateBlockSolid : CEngineStateBlock { CEngineStateBlockSolid() { } };
struct CEngineStateBlockSolid2X : CEngineStateBlock { CEngineStateBlockSolid2X() { } };
struct CEngineStateBlockAlpha : CEngineStateBlock { CEngineStateBlockAlpha() { } };
struct CEngineStateBlockAlpha2X : CEngineStateBlock { CEngineStateBlockAlpha2X() { } };
struct CEngineStateBlockAdditiveAlpha : CEngineStateBlock { CEngineStateBlockAdditiveAlpha() { } };
struct CEngineStateBlockAddSmoothAlpha : CEngineStateBlock { CEngineStateBlockAddSmoothAlpha() { } };
struct CEngineStateBlockAlpha2XTFactor : CEngineStateBlock { CEngineStateBlockAlpha2XTFactor() { } };
struct CEngineStateBlockDiffuseOnly : CEngineStateBlock { CEngineStateBlockDiffuseOnly() { } };
struct CEngineStateBlockDiffuse2X : CEngineStateBlock { CEngineStateBlockDiffuse2X() { } };
struct CEngineStateBlockDiffuseEnv2X : CEngineStateBlock { CEngineStateBlockDiffuseEnv2X() { } };
struct CEngineStateBlocks : CStateBlocksBase_00b59710 { CEngineStateBlockSolid m_00; CEngineStateBlockSolid2X m_01; CEngineStateBlockAlpha m_02; CEngineStateBlockAlpha2X m_03; CEngineStateBlockAdditiveAlpha m_04; CEngineStateBlockAddSmoothAlpha m_05; CEngineStateBlockAlpha2XTFactor m_06; CEngineStateBlockDiffuseOnly m_07; CEngineStateBlockDiffuse2X m_08; CEngineStateBlockDiffuseEnv2X m_09;
    CEngineStateBlocks() : CStateBlocksBase_00b59710(), m_00(), m_01(), m_02(), m_03(), m_04(), m_05(), m_06(), m_07(), m_08(), m_09() { } };
int main() {
    CEngineStateBlocks s; if (g_base != 1 || g_blocks != 10 || (char*)&s.m_09 - (char*)&s.m_00 != 9 * sizeof(void*)) { std::printf("00b28bc0_TEST FAIL\n"); return 1; }
    std::printf("00b28bc0_TEST PASS\n"); return 0;
}