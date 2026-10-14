// CSeaQuadElement::CSeaQuadElement(CSeaQuadOwner*, void*) -- EH-framed constructor: the owner and the data stored, two three-dword link members zeroed inline (EH states 0 and 1), then the owner's registry at +0x74 adds `this` through virtual slot 9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E6D0, oracle length 103 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[owner] ; push esi ; mov esi,ecx ; mov ecx,[data] ; mov [esi],eax ; xor eax,eax ; mov [esi+4],ecx ; mov [esp+4],esi ; mov [esi+8],eax ; mov [esi+0Ch],eax ; mov [esi+10h],eax ; mov [esp+10h],eax ; mov [esi+14h],eax ; mov [esi+18h],eax ; mov [esi+1Ch],eax ; mov eax,[esi] ; mov edx,[eax+74h] ; lea ecx,[eax+74h] ; push esi ; mov byte ptr [esp+14h],1 ; call [edx+24h] ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// The two link members have inline zeroing constructors and out-of-line destructors, which is
// what gives EH state 0 after the first and state 1 (narrowed to a byte) after the second; the
// body then registers the element. The class comes from the manifest label; the rest is descriptive.
struct CSeaQuadElement;
struct CSeaQuadRegistry_00c1e6d0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CSeaQuadElement* element);   // slot 9
};

struct CSeaQuadOwner_00c1e6d0 {
    unsigned char _pad[0x74];
    CSeaQuadRegistry_00c1e6d0 m_registry;  // +0x74
};

struct CSeaQuadLink_00c1e6d0 {             // three pointers, zeroed inline, released out of line
    void* m_pPrev;
    void* m_pNext;
    void* m_pList;
    CSeaQuadLink_00c1e6d0() : m_pPrev(0), m_pNext(0), m_pList(0) { }
    ~CSeaQuadLink_00c1e6d0();
};

struct CSeaQuadElement {
    CSeaQuadOwner_00c1e6d0* m_pOwner;      // +0
    void* m_pData;                         // +4
    CSeaQuadLink_00c1e6d0 m_link;          // +8
    CSeaQuadLink_00c1e6d0 m_activeLink;    // +0x14
    CSeaQuadElement(CSeaQuadOwner_00c1e6d0* owner, void* data);
};

CSeaQuadElement::CSeaQuadElement(CSeaQuadOwner_00c1e6d0* owner, void* data)
    : m_pOwner(owner), m_pData(data), m_link(), m_activeLink()
{
    m_pOwner->m_registry.Add(this);
}