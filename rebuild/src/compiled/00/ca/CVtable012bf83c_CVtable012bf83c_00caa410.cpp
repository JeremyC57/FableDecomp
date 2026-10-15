// CVtable_012bf83c::~CVtable_012bf83c() -- EH-framed destructor: both vtables, the second interface's stop method on itself, the entry queue released, then the base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA410, length 96 bytes. Built under /EHsc.
// Leaf: ??1CVtable_012bf83c
// Flags: /EHsc
// Retail bytes: push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea eax,[esi+0Ch] ;
//               mov [esp+4],esi ; mov dword ptr [esi],offset vftable(0x012BF83C) ; mov dword ptr [eax],offset vftable(0x012BF810) ; push eax ;
//               mov dword ptr [esp+14h],0 ; call Stop ; mov ecx,esi ; call DeallocateScriptEntries ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ;
//               call ~base ; (EH epilogue) ; ret
// Callees: the second interface's slot 6 @ 0x00CAA160 (a COM `__stdcall` method: the interface pointer is pushed and converted
// back with `lea eax,[esi-0Ch]`), DeallocateScriptEntries @ 0x00CAA3C0, the base destructor @ 0x00CA9D00 (closes the +0x30 handle,
// releases the +0x58 COM pointer, deletes the +0x10 critical section, then the module reference at +4 @ 0x00CACAF0).
// The manifest label "~CScriptDefinitionManager" is propagated; the class is named by its primary vtable 0x012BF83C (no-RTTI
// module): slot 0 QueryInterface, 1 AddRef, 2 Release, 3 the scalar deleting destructor 0x00CABC60 (calls this function); the
// interface at +0xc (vtable 0x012BF810) forwards QueryInterface/AddRef/Release to the owner through [this-8].
// Shape: in a destructor the call to the virtual Stop is bound statically (VC7.1 devirtualises it) and the `this` adjustment to
// the interface is computed once (`lea eax,[esi+0Ch]`) for the vtable store and the push; EH state 0 covers the body because the
// base destructor is out of line.
struct CModuleReference_00cacaf0 {             // an empty member whose destructor drops the module's reference count
    ~CModuleReference_00cacaf0();              // 0x00CACAF0
};

struct IUnknownRetail {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

struct IStreamCallback_012bf810 : IUnknownRetail {   // the second interface
    virtual long __stdcall v03() = 0;
    virtual long __stdcall v04() = 0;
    virtual long __stdcall v05() = 0;
    virtual long __stdcall Stop() = 0;         // slot 6
};

struct CUnknownRetail : IUnknownRetail {       // 0x0c bytes: vptr, module reference, reference count
    virtual ~CUnknownRetail();                 // slot 3
    CModuleReference_00cacaf0 m_moduleReference;   // +0x04
    long                      m_refCount;          // +0x08
};

struct CBase_00ca9d00 : CUnknownRetail, IStreamCallback_012bf810 {   // the base (interface at +0x0c)
    unsigned char m_body[0x60 - 0x10];         // +0x10 critical section, queue, handles, COM pointer
    ~CBase_00ca9d00();                         // 0x00CA9D00
};

struct CVtable_012bf83c : CBase_00ca9d00 {     // vtables 0x012BF83C / 0x012BF810
    void* m_pBlock;                            // +0x60
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // 0x00CA9D70
    virtual unsigned long __stdcall AddRef();  // 0x00CACD00
    virtual unsigned long __stdcall Release(); // 0x00CACD20
    virtual long __stdcall v03();
    virtual long __stdcall v04();
    virtual long __stdcall v05();
    virtual long __stdcall Stop();             // 0x00CAA160
    void DeallocateScriptEntries();            // 0x00CAA3C0
    virtual ~CVtable_012bf83c();
};

CVtable_012bf83c::~CVtable_012bf83c()
{
    Stop();
    DeallocateScriptEntries();
}