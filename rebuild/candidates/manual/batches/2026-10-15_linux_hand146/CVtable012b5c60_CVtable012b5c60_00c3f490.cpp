// CVtable_012b5c60::~CVtable_012b5c60() -- destructor of the no-RTTI 0x00C5xxxx-module class with vtable 0x012B5C60: own vtable; unless the stopped flag (bit 31 of +0x2c) is set its own Stop() override runs (devirtualised); the embedded member at +0xb4 (an empty polymorphic class, vtable 0x012B5868) is destroyed; then the base destructor
// Leaf: ??1CVtable_012b5c60
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F490, length 101 bytes (EH frame; built under /EHsc).
// Callees: CVtable_012b5c60::Stop() @ 0x00C5A1F0 (slot 2 override; the manifest calls it CMap::Update), the base destructor
//   ~CVtable_012B5C10 @ 0x00C3F3D0 (landed; manifest label ~CParticleEffect is propagated).
// Identity: the manifest label "CMap::Destructor" is wrong -- the vtable store 0x012B5C60 names the class (the landed 0x00C3F3D0 header
//   already identifies 0x00C3F490 as this derived destructor; landed GetDataSize 0x00C3F510 is its slot 13, m_key at +0xb8).
// Shape: same IsRunning() accessor as the base (`shr eax,1Fh ; not al ; test al,1`); the member's inline virtual destructor only
//   restores its vtable (`mov [esi+0B4h],012B5868h`); EH state 1 while base + member are live, -1 before the base destructor call.
struct CListNode {                                 // base of the base: intrusive list node
    virtual ~CListNode();                          // 0x00C4CF40
    CListNode* m_prev;                             // +0x04
    CListNode* m_next;                             // +0x08
};
struct CVtable_012B5C10 : CListNode {
    unsigned char _body[0x20];                     // +0x0c
    unsigned long m_state   : 31;
    unsigned long m_stopped : 1;                   // +0x2c bit 31
    virtual void v01();
    virtual void Stop();                           // slot 2 (0x00C59710)
    bool IsRunning() const { return !m_stopped; }
    virtual ~CVtable_012B5C10();                   // 0x00C3F3D0
};
struct CVtable_012b5868 {
    virtual ~CVtable_012b5868() { }
};
struct CVtable_012b5c60 : CVtable_012B5C10 {
    unsigned char    _pad30[0xb4 - 0x30];
    CVtable_012b5868 m_member;                     // +0xb4
    long             m_key;                        // +0xb8
    virtual void Stop();                           // 0x00C5A1F0
    virtual ~CVtable_012b5c60();
};
CVtable_012b5c60::~CVtable_012b5c60()
{
    if (IsRunning())
        Stop();
}
