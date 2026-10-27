// CThingDeadCreature::Alloc() -- class-factory hook: CThing's pooled operator new(sizeof(CThingDeadCreature) = 0xF0, false), then the out-of-line default constructor
// Leaf: ?Alloc@CThingDeadCreature@@SIPAVCClass@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00527300, 26 bytes, plain `ret` (`static CClass* __fastcall`).
// Identity: CThingManager::InitThingTypes 0x00522A20 stores this address with the "CREATURE" type name (0x00523200:
//   `mov [esp+1Ch],0x527300` after the CCharString("CREATURE") temporary), and its constructor is the landed
//   CThingDeadCreature::CThingDeadCreature() 0x00835C20 (vtable 0x01272B04). Ego_r.pdb / FableWin.pdb
//   ?Alloc@CThingDeadCreature@@SIPAVCClass@@XZ; the next row 0x00527320 is the landed CThingObject::Alloc.
// Callees: CThing::operator new(unsigned int, bool) @ 0x004C7380 (`??2CThing@@KIPAXI_N@Z`, protected static __fastcall:
//   size in ecx, the bool in dl), CThingDeadCreature::CThingDeadCreature() @ 0x00835C20.
// Shape: the landed CThingObject::Alloc 0x00527320: `new (false) CThingDeadCreature` keeps its null test and tail-jumps to
//   the constructor (which returns `this`).
class CClass {
public:
    virtual void v00();
};
class CThing : public CClass {
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380
public:
    unsigned char _body[0xC0 - 4];
};
class CThingDeadCreature : public CThing {
public:
    unsigned char _body2[0xF0 - 0xC0];                                  // sizeof(CThingDeadCreature) = 0xF0 in retail
    CThingDeadCreature();                                                // 0x00835C20
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingDeadCreature::Alloc()
{
    return new (false) CThingDeadCreature;
}