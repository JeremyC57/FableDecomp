// CIntelligentPointer_00c1fd60::Validate() -- slot 3 of the item: validate the item's table member (GetTable(), slot 5) through the table's own Validate (slot 9)
// Leaf: ?Validate@CIntelligentPointer_00c1fd60@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1FDF0, 12 bytes, thiscall, plain `ret` through the tail call: `mov eax,[ecx] ;
//   call [eax+14h] ; mov edx,[eax] ; mov ecx,eax ; jmp [edx+24h]`.
// Identity: slot 3 of CIntelligentPointer_00c1fd60's vtable 0x012B35B8 (the only reference, cell 0x012B35C4); the class is the one the landed constructor
//   0x00C1FD60 builds (base vtable 0x012B354C, then 0x012B35B8). The vtable's 9 slots: 0/1 `lea eax,[ecx+4]`, 2 the key accessor
//   (0x00C1FDE0: `lea eax,[ecx+0x18]`), 3 this body, 4 the key kind (0x00C1FDD0: `return 3`), 5 GetTable (`lea eax,[ecx+8]`:
//   the table member at +8 (16-byte table member, ctor 0x00C34B30)), 6..7 inherited, 8 the deleting destructor. The base CPointerBase_00c1fd60 carries a byte-identical
//   slot 3 of its own (0x00C1FB90). Slot 9 of the table vtables (abstract 0x012B3444 and its four derived tables) is the landed
//   CItemTable::Validate 0x00C348F0 (the row/column walk), so the forwarder takes its name.
// libpk has no PDB or RTTI: the class names are the landed descriptive ones, the method name is descriptive (behaviour).
// Shape: a plain virtual override calling two virtuals; VC7.1 tail-jumps the second.
class CItemTable {                                  // the table interface (vtable 0x012B3444)
public:
    virtual unsigned long GetType() = 0;            // slot 0
    virtual long v01();
    virtual long v02();
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Validate();                        // slot 9 (+0x24): 0x00C348F0
};

struct CPointerBase_00c1fd60 {                                     // vtable 0x012B354C
    virtual void* v00();                            // slot 0
    virtual void* v01();                            // slot 1
    virtual const void* GetKey();                   // slot 2
    virtual void Validate();                        // slot 3 (+0x0c): 0x00C1FB90
    virtual unsigned long GetKeyType();             // slot 4
    virtual CItemTable* GetTable();                 // slot 5 (+0x14)
};

struct CIntelligentPointer_00c1fd60 : public CPointerBase_00c1fd60 {                      // vtable 0x012B35B8
    virtual void Validate();                        // slot 3
};

void CIntelligentPointer_00c1fd60::Validate()
{
    GetTable()->Validate();
}