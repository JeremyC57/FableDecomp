// CThingObject::Alloc() -- class-factory hook: CThing's pooled operator new(sizeof(CThingObject) = 0xE0, false), then the out-of-line default constructor
// Leaf: ?Alloc@CThingObject
// Retail: Fable.exe (TLC, VC7.1) @ 0x00527320, 26 bytes, plain `ret` (`static CClass* __fastcall`, FableWin.pdb
//   `?Alloc@CThingObject@@SIPAVCClass@@XZ`; the manifest label "Alloc" is right, the module was missing).
// Callees: CThing::operator new(unsigned int, bool) @ 0x004C7380 (FableWin.pdb `??2CThing@@KIPAXI_N@Z`, a protected
//   static __fastcall: size in ecx, the bool in dl), CThingObject::CThingObject() @ 0x007031C0 (landed).
// Shape: `xor dl,dl ; mov ecx,0E0h ; call new ; test ; je ; mov ecx,eax ; jmp ctor ; xor eax,eax ; ret` is a
//   placement-argument new-expression `new (false) CThingObject` whose null test VC7.1 keeps (operator new may
//   return 0) and whose constructor call becomes a sibling tail jump (the constructor returns `this`).
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
class CThingObject : public CThing {
public:
    unsigned char _body2[0xE0 - 0xC0];                                  // sizeof(CThingObject) = 0xE0 in retail
    CThingObject();                                                      // 0x007031C0
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingObject::Alloc()
{
    return new (false) CThingObject;
}