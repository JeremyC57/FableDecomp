// CSoundBankLH::Unload_00a0b6c0() -- release the bank object (its slot 0) and clear the pointer (descriptive name)
// Leaf: ?Unload_00a0b6c0@CSoundBankLH@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B6C0, 23 bytes, thiscall.
// Identity (proven, chunk c670): slot 2 of vtable 0x0129C9DC (RTTI CSoundBankLH): `if (BankObject) { BankObject->slot0();
//   BankObject = 0; }` on CSoundBankLH +0x10, the body ~CSoundBankLH 0x00A0B9A0 expands; class proven, member name
//   descriptive (the sound module is in neither PDB; c670 decision: stays descriptive).
// Naming wave 16: landed as the free __fastcall ClearAllChildPrimitives(CEnginePrimitiveRenderer*, ...) (catalog
//   CEnginePrimitiveRenderer::ClearAllChildPrimitives); respelled on the class-proven descriptive member (name-only).
class CSoundBankObject_00a0b6c0 {                       // the bank object's interface (descriptive)
public:
    virtual void Release_00a0b6c0();                    // slot 0
};
class CSoundBankLH {
public:
    virtual void v00();
    virtual void v01();
    virtual void Unload_00a0b6c0();                     // slot 2
    unsigned char _pad_0x04[0xc];
    CSoundBankObject_00a0b6c0* BankObject;              // +0x10 (descriptive)
};

void CSoundBankLH::Unload_00a0b6c0()
{
    CSoundBankObject_00a0b6c0* c = BankObject;
    if (c) {
        c->Release_00a0b6c0();
        BankObject = 0;
    }
}
