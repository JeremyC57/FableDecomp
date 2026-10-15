// NUISystem::CSpellContainer::CSpellContainer(long owner) -- CChangingStateComponent(owner), the three vtables, empty BaseSpells vector and NewSpell pair, then the position offset copied from the def (GetDataFromDef inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00545720, length 136 bytes (`ret 4`).
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50, the component's virtual slot 108 (+0x1b0,
// GetDef, CDefPointer by hidden pointer into the dead `owner` slot), the def's virtual slot 1 when its count reaches zero.
// Vtables 0x0124786C / 0x01247848 / 0x01247840 = NUISystem::CSpellContainer (rtti-vtables.tsv).
// Layout: Ego_r.pdb CSpellContainer minus 0x30 (retail CChangingStateComponent 0x15C): BaseSpells +0x15c (12-byte vector),
// NewSpell +0x168, EmptySlot +0x170 (not initialised), PositionOffset +0x174; the def's offset C2DVector at +0x142 (packed).
// Shape: the def read sits in an inline GetDataFromDef() helper (a virtual call written in the ctor body itself is bound
// statically); the CDefPointer dtor calls the def's inline ReleaseTemporary() with NO null test, whose DecRef/GetRefCount
// accessors live on the ref-count base at +4 -- that base adjustment is retail's `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ;
// dec edx ; cmp edx,edi ; mov [eax],edx` release (the shape GOTCHAS lists as a lane gap).
// Leaf: ??0CSpellContainer
namespace NUISystem {

struct CComponentInterface { virtual void v000(); };
struct CObserver_Retail { virtual void ProcessEvent(); unsigned char _body[0x10]; };
struct CEventSinkInterface { virtual void v0(); };

struct CChangingStateComponent : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner);         // 0x0052CC50
};

struct C2DVector { float X, Y; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                      // slot 1
};
#pragma pack(push, 1)
struct CSpellContainerDef : CDefVirtual, CRefCounted {
    unsigned char _def[0x13a];                   // +0x08
    C2DVector     PositionOffset;                // +0x142 (packed def)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
struct CDefPointer {
    CSpellContainerDef* m_p;
    ~CDefPointer()
    {
        m_p->ReleaseTemporary();
    }
    CSpellContainerDef* operator->() const { return m_p; }
};

template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_end;
    CRetailVector() : m_start(0), m_finish(0), m_end(0) {}
};
struct CComponent;
struct CCountedPointerComponent { CComponent* Data; void* Info; };
struct SNewSpell {
    CComponent*   first;
    unsigned long second;
    SNewSpell() : first(0), second(0) {}
};

struct CSpellContainer : CChangingStateComponent {   // vtables 0x0124786C / 0x01247848 / 0x01247840
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
#define V(n) virtual void v##n();
    V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107)
#undef V
    virtual CDefPointer GetDef();                // slot 108 (+0x1b0)
    CRetailVector<CCountedPointerComponent> BaseSpells;   // +0x15c
    SNewSpell     NewSpell;                      // +0x168
    long          EmptySlot;                     // +0x170
    C2DVector     PositionOffset;                // +0x174
    CSpellContainer(long owner);
    void GetDataFromDef()
    {
        CDefPointer def = GetDef();
        PositionOffset = def->PositionOffset;
    }
};

CSpellContainer::CSpellContainer(long owner)
    : CChangingStateComponent(owner)
{
    GetDataFromDef();
}

}