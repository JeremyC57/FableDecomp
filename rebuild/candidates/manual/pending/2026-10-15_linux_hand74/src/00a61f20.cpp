// CShaderBankStateBlock::GetDefault(CBankStateBlock& block) -- fill a shader bank state block with the defaults: state cleared, shader optimisation on, default shader target
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A61F20, 27 bytes (`?GetDefault@CShaderBankStateBlock@@UAEXAAVCBankStateBlock@@@Z`,
// Ego_r.pdb; vtable 0x0129D630 slot 1). No callees.
//
// Shape: the 8-byte state is cleared with an intrinsic `memset` (VC7.1 copies the destination into esi and
// stores the zero register twice -- a value-initialised temporary assignment gives two plain stores
// through the argument register instead), then EnableShaderOptimisation = true and ShaderTarget = 0 (a
// second store to the first word, through the address register).
// Layout = Ego_r.pdb CShaderBankStateBlock (12 bytes): CBankStateBlock (vptr) + CStateBlock State +4
// {EShaderTarget ShaderTarget +0, bool EnableShaderOptimisation +4}. The block argument is the base class
// reference, cast down to the shader block. No engine header exists for CShaderBankStateBlock.
// Leaf: ?GetDefault@CShaderBankStateBlock
#include <string.h>

enum EShaderTarget {
    SHADER_TARGET_DEFAULT = 0
};

struct CBankStateBlock {
    virtual void CreateCRC();                    // slot 0
    virtual void GetDefault(CBankStateBlock& block);   // slot 1
};

struct CShaderBankStateBlock : CBankStateBlock {
    struct CStateBlock {
        EShaderTarget ShaderTarget;              // +0x00
        bool          EnableShaderOptimisation;  // +0x04
    };
    CStateBlock State;                           // +0x04
    virtual void GetDefault(CBankStateBlock& block);
};

void CShaderBankStateBlock::GetDefault(CBankStateBlock& block)
{
    CShaderBankStateBlock& shaderBlock = static_cast<CShaderBankStateBlock&>(block);
    memset(&shaderBlock.State, 0, sizeof(shaderBlock.State));
    shaderBlock.State.EnableShaderOptimisation = true;
    shaderBlock.State.ShaderTarget = SHADER_TARGET_DEFAULT;
}
