// CClothingSuitMap_00c19d90::VisitPart(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the node found out of line (@ 0x00C19680) accepts two 8-byte stack visitors holding the second argument: the first (vtable 0x012B2F88) through virtual slot 10 of its member at +0x10, the second (vtable 0x012B2F90) through its own virtual slot 10; both visitors' inlined base destructors store the base vtable (0x012B260C); returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19D90, oracle length 170 bytes. Built under /EHsc.
// Retail bytes:  ... call Find ; mov esi,eax ; mov eax,[arg] ; mov dword ptr [esp+0Ch],offset vtableA ; mov [esp+10h],eax ; mov dword ptr [esp+4],offset vtableB ; mov [esp+8],eax ; mov edx,[esi+10h] ; lea ecx,[esi+10h] ; lea eax,[esp+0Ch] ; push eax ; mov byte ptr [esp+28h],2 ; call [edx+28h] ; mov edx,[esi] ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call [edx+28h] ; mov eax,offset basevtable ; lea ecx,[esp+14h] ; mov [esp+4],eax ; mov [esp+0Ch],eax ; mov dword ptr [esp+24h],-1 ; call ~CScopedLock ; ... xor eax,eax ; ...
// Two visitor locals (states 1 and 2) whose derived implicit destructors store nothing and whose
// base destructor stores the base vtable inline. The manifest label is propagated; the class is
// named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CVisitorBase_00c19d90 {             // vtable 0x012B260C
    virtual void v00();
    ~CVisitorBase_00c19d90() { }
};

struct CPartVisitor_00c19d90 : CVisitorBase_00c19d90 {   // vtable 0x012B2F88
    void* m_pArgument;
    CPartVisitor_00c19d90(void* argument) : m_pArgument(argument) { }
};

struct CNodeVisitor_00c19d90 : CVisitorBase_00c19d90 {   // vtable 0x012B2F90
    void* m_pArgument;
    CNodeVisitor_00c19d90(void* argument) : m_pArgument(argument) { }
};

struct CPart_00c19d90 {                    // polymorphic member at +0x10 of the node
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Accept(CVisitorBase_00c19d90* visitor);   // slot 10
};

struct CNode_00c19d90 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Accept(CVisitorBase_00c19d90* visitor);   // slot 10
    unsigned char _pad[0xc];
    CPart_00c19d90 m_part;                 // +0x10
};

struct CClothingSuitMap_00c19d90 {
    CNode_00c19d90* Find(void* key);       // 0x00C19680
    long VisitPart(void* key, void* argument);
};

long CClothingSuitMap_00c19d90::VisitPart(void* key, void* argument)
{
    CScopedLock lock(&g_lock_0143bb14);
    CNode_00c19d90* node = Find(key);
    CPartVisitor_00c19d90 partVisitor(argument);
    CNodeVisitor_00c19d90 nodeVisitor(argument);
    node->m_part.Accept(&partVisitor);
    node->Accept(&nodeVisitor);
    return 0;
}
