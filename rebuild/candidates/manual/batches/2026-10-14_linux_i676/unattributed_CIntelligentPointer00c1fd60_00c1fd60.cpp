// CIntelligentPointer_00c1fd60::CIntelligentPointer_00c1fd60(void*, void*) -- EH-framed constructor: empty destructible base (EH state 0), item pointer at +4, 16-byte member at +8 built out of line (@ 0x00C34B30), own vtable, then the member at +0x18 built from the first argument (@ 0x00C0DAB0, state 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1FD60, oracle length 105 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[item] ; lea ecx,[esi+8] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset base vtable ; mov [esi+4],eax ; call member8 ctor ; mov ecx,[source] ; push ecx ; lea ecx,[esi+0x18] ; mov dword ptr [esp+14h],1 ; mov dword ptr [esi],offset vtable ; call memberN ctor ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Two-level EH-framed constructor: the base class (a polymorphic root at +0 with an out-of-line
// destructor and no vtable store of its own (`novtable`), which is what puts the frame in EH state 0
// before any member exists -- its funclet destroys it on `this` with no offset; the base's own
// destructor is out of line too, so state 1's funclet destroys the whole
// base through it rather than the member; its own vtable; the item
// pointer at +4; a 16-byte member at +8 default-constructed out of line @ 0x00C34B30) and the
// class itself (vtable, state 1, the member at +0x18 constructed from the first argument @
// 0x00C0DAB0). `this` is kept in the frame for the unwind funclets. Built under /EHsc (the
// `__ehhandler` is not compared; the funclets land through the funclet pass). The manifest
// label names the class; the member types are opaque. One of 4 bodies of this shape.
struct CMember8 {                          // member at +8, default-constructed out of line
    unsigned char _body[0x10];
    CMember8();                            // 0x00C34B30
    ~CMember8();
};

struct CMemberN {                          // member at +0x18, constructed from the first argument
    unsigned char _body[4];
    CMemberN(void* source);                // 0x00C0DAB0
    ~CMemberN();
};

struct __declspec(novtable) CDestructible { // polymorphic root at +0 (no vtable store of its own) with an out-of-line
    virtual void v00();                    // destructor: EH state 0 while the first member is built; its funclet
    ~CDestructible();                      // destroys it on `this` (+0), which is why the root owns the vptr
};

struct CPointerBase_00c1fd60 : CDestructible {     // vtable 0x012B354C
    void* m_pItem;                         // +4
    CMember8 m_member8;                    // +8
    CPointerBase_00c1fd60(void* item) : m_pItem(item), m_member8() { }
    ~CPointerBase_00c1fd60();                   // out of line: the unwind funclet for state 1 destroys the whole base through it
};

struct CIntelligentPointer_00c1fd60 : CPointerBase_00c1fd60 {             // vtable 0x012B35B8
    CMemberN m_memberN;                    // +0x18
    CIntelligentPointer_00c1fd60(void* source, void* item);
};

CIntelligentPointer_00c1fd60::CIntelligentPointer_00c1fd60(void* source, void* item)
    : CPointerBase_00c1fd60(item), m_memberN(source)
{
}
