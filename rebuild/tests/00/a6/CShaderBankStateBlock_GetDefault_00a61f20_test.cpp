// Behaviour test for CShaderBankStateBlock::GetDefault (0x00A61F20): the target block gets target 0 and optimisation enabled.
#include <cstdio>
#include <string.h>
enum EShaderTarget { SHADER_TARGET_DEFAULT = 0, SHADER_TARGET_OTHER = 3 };
class CBankStateBlock { public: virtual ~CBankStateBlock() {} virtual void GetDefault(CBankStateBlock& block) = 0; };
struct CShaderBankStateBlock : CBankStateBlock {
    struct CStateBlock { EShaderTarget ShaderTarget; bool EnableShaderOptimisation; };
    CStateBlock State;
    virtual void GetDefault(CBankStateBlock& block)
    {
        CShaderBankStateBlock& shaderBlock = static_cast<CShaderBankStateBlock&>(block);
        memset(&shaderBlock.State, 0, sizeof(shaderBlock.State));
        shaderBlock.State.EnableShaderOptimisation = true;
        shaderBlock.State.ShaderTarget = SHADER_TARGET_DEFAULT;
    }
};
int main()
{
    CShaderBankStateBlock source, target;
    source.State.ShaderTarget = SHADER_TARGET_OTHER; source.State.EnableShaderOptimisation = false;
    target.State.ShaderTarget = SHADER_TARGET_OTHER; target.State.EnableShaderOptimisation = false;
    source.GetDefault(target);
    if (target.State.ShaderTarget != SHADER_TARGET_DEFAULT || !target.State.EnableShaderOptimisation || source.State.ShaderTarget != SHADER_TARGET_OTHER) { std::printf("00a61f20_TEST FAIL\n"); return 1; }
    std::printf("00a61f20_TEST PASS\n");
    return 0;
}