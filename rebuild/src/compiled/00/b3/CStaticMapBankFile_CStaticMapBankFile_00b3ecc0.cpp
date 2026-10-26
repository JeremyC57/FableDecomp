// CStaticMapBankFile::CStaticMapBankFile() -- base CBankFile constructor, own vtable, then the one-argument setter @ 0x009D5230 with true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3ECC0, length 27 bytes (+ int3 padding).
// Callees: CBankFile::CBankFile @ 0x009D5F80 (stores vtable 0x0129B8D4 and zeroes +0x110/+0x114), the
// empty-bodied `ret 4` setter @ 0x009D5230 (manifest "$E2", a propagated label; one bool argument pushed as 1).
// Vtable 0x012A1EE4 is CStaticMapBankFile (rtti-vtables.tsv); the manifest module
// CCombatAbilityUseProjectileWeaponDef is a propagated label and wrong.
// Shape: `push esi ; mov esi,ecx ; call base ; push 1 ; mov ecx,esi ; mov [esi],vtable ; call setter ; mov eax,esi`.
struct CBankFile_Retail {              // engine/CBankFile.h exists but is not used: the retail base is 0x118 bytes; only its ctor and the empty setter are used
    virtual void v00();
    unsigned char _body[0x118 - 4];
    CBankFile_Retail();                                // 0x009D5F80
    void SetIsStaticMap(bool isStaticMap);      // 0x009D5230 (empty body: `ret 4`)
};

class CStaticMapBankFile : public CBankFile_Retail { public:
    virtual void v00();                         // own vtable 0x012A1EE4
    CStaticMapBankFile();
};

CStaticMapBankFile::CStaticMapBankFile()
{
    SetIsStaticMap(true);
}