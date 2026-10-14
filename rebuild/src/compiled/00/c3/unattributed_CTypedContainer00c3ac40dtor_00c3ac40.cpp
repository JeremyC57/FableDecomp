// CTypedContainer_00c3ac40::~CTypedContainer_00c3ac40() -- EH-framed destructor: every item the container still yields (virtual slot 11) is removed (virtual slot 8) and freed through the global operator delete (@ 0x00BFE9BC), then the container base's vtable (0x0112B5578) is stored, its map member at +8 destroyed out of line (@ 0x00C19250, state 1) and the root vtable (0x012B1AB0) stored by the inlined root destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3AC40, oracle length 124 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov eax,[esi] ; mov dword ptr [esp+14h],0 ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; je done ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+20h] ; push edi ; call operator delete ; mov eax,[esi] ; add esp,4 ; mov ecx,esi ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; jne loop ; done: mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov dword ptr [esp+14h],1 ; call ~CMap ; mov ecx,[esp+0Ch] ; mov dword ptr [esi],offset rootvtable ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The most-derived class stores no vtable of its own (`__declspec(novtable)`), so the drain loop
// runs before the container's vtable store; the calls stay virtual because the body runs through
// the container's vtable in a novtable class. The manifest label is propagated; the classes are
// named by address.
struct CItem_00c3ac40 { };                  // trivially destructible: `delete` is operator delete alone

struct CContainerBase_00c3ac40 {             // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CItem_00c3ac40* item);   // slot 8
    virtual void v09(); virtual void v10();
    virtual CItem_00c3ac40* Next();            // slot 11
    long m_count;                          // +4
    __forceinline CItem_00c3ac40* NextItem() { return Next(); }          // keeps the call virtual inside the destructor
    __forceinline void RemoveItem(CItem_00c3ac40* item) { Remove(item); }
    ~CContainerBase_00c3ac40() { }
};

struct CMap_00c3ac40 {
    unsigned char _body[0x28];
    ~CMap_00c3ac40();                          // 0x00C19250
};

struct CContainer_00c3ac40 : CContainerBase_00c3ac40 {   // vtable 0x0112B5578
    CMap_00c3ac40 m_map;                    // +8
    ~CContainer_00c3ac40() { }
};

struct __declspec(novtable) CTypedContainer_00c3ac40 : CContainer_00c3ac40 {
    ~CTypedContainer_00c3ac40();
};

CTypedContainer_00c3ac40::~CTypedContainer_00c3ac40()
{
    CItem_00c3ac40* item = NextItem();
    while (item) {
        RemoveItem(item);
        delete item;
        item = NextItem();
    }
}