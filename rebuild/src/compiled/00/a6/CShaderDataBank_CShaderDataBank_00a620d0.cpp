// CShaderDataBank::~CShaderDataBank() -- close the shader entries, then destroy StateBlock and the Shaders array and chain to ~CBankFile
// Leaf: ??1CShaderDataBank@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A620D0, 51 bytes, thiscall, tail `jmp ~CBankFile`.
// Identity (proven): it stores vtable 0x0129D644 (RTTI .?AVCShaderDataBank@@; the landed ctor 0x00A62020 and ??_G 0x00A62860
//   store the same one) and its callers are CShaderDataBank arrays of stride 0x128: CEngineResourceManager's destructor
//   0x00B3BAE4 / 0x00B3BB08 (the array at +0x2898 walked down 0x1B times) and 0x009C309C. FableWin.pdb spells
//   ??1CShaderDataBank@@UAE@XZ (donor 0x0336E950, after ??0 / ??_E and the CShaderBankStateBlock pair). The catalog label
//   "CParticleEffect::~CParticleEffect" is propagated.
// Callees: CShaderDataBank::CloseShaders 0x00A62050 (Ego_r ?CloseShaders@CShaderDataBank@@AAEXXZ: private; the landed
//   CShaderDataBank::Close 0x00A62110 calls it before CBankFile::Close), free 0x00BFEA14 (the Shaders vector's storage),
//   CBankFile::~CBankFile 0x009CE180.
// Shape: a user destructor `{ CloseShaders(); }` (the vtable store, then the call on the untouched ecx); then the members in
//   reverse order: StateBlock's implicit ~CShaderBankStateBlock drops its own store and keeps the inline ~CBankStateBlock's
//   (0x0129C82C, RTTI .?AVCBankStateBlock@@), the Shaders STLport vector of pointers frees its block under the null test;
//   then the base dtor.
// Layout: retail CBankFile is 0x110 bytes (STLport containers); engine/CBankFile.h is on an engine-header hold (flattened,
//   no vtable), so the base is modelled locally as polymorphic with its out-of-line destructor (header debt). Shaders
//   (CArray<CShaderBankEntry*>, 12 bytes) +0x110 and StateBlock (CShaderBankStateBlock, 12 bytes) +0x11C, as the landed
//   ctor 0x00A62020 builds them; the Ego_r-only ShaderPreParser / EntryParserStates / IntermediateFileFolder members are
//   absent from the retail class (sizeof 0x128 = the callers' stride). engine/CShaderDataBank.h carries those Ego_r members.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

class CBankFile {                                           // vtable 0x0129B54C, retail 0x110 bytes (local model: engine/CBankFile.h is on hold)
public:
    virtual ~CBankFile();                                   // 0x009CE180
    unsigned char _body[0x110 - 4];
};
class CBankStateBlock {                                     // vtable 0x0129C82C
public:
    virtual ~CBankStateBlock() { }
};
enum EShaderTarget { };                                     // Ego_r W4EShaderTarget@@; its values do not reach the bytes
class CShaderBankStateBlock : public CBankStateBlock {      // vtable 0x0129D630, 0xC bytes (Ego_r)
public:
    class CStateBlock {                                     // Ego_r CShaderBankStateBlock::CStateBlock, 8 bytes
    public:
        EShaderTarget ShaderTarget;                         // +0x00
        bool          EnableShaderOptimisation;             // +0x04
    };
    CStateBlock State;                                      // +0x04
};
class CShaderBankEntry;

class CShaderDataBank : public CBankFile {                  // vtable 0x0129D644, retail 0x128 bytes
public:
    CShaderDataBank();                                      // 0x00A62020
    virtual ~CShaderDataBank();
    CArray<CShaderBankEntry*> Shaders;                      // +0x110
    CShaderBankStateBlock     StateBlock;                   // +0x11c
private:
    void CloseShaders();                                    // 0x00A62050
};

CShaderDataBank::~CShaderDataBank()
{
    CloseShaders();
}