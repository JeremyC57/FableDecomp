// NUISystem::CSprite::GetDynamicSize() const -- the size used for dynamic layout: just the component's GetSize()
// Leaf: ?GetDynamicSize@CSprite@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C660, 17 bytes, thiscall, hidden return pointer, `ret 4`.
// Identity (proven): slot 111 of vtable 0x0122F5D4 (RTTI .?AVCSprite@NUISystem@@; inherited at slot 111 by CSlider 0x01248D04, CListArrow 0x01248F94, CMorphingSprite 0x0124A49C, CMouseCursor 0x0124C22C); slot 23 of the same vtable is CSprite::GetSize 0x0041A850; Ego_r.pdb ?GetDynamicSize@CSprite@NUISystem@@UBE?AVC2DVector@@XZ.
// Callees: the virtual GetSize() const at slot 23 (`call [eax+5Ch]`).
// Shape (family M020, representative 0x0041C660): `return GetSize();` -- the hidden return pointer is forwarded to the
//   virtual call and returned in eax.
// Header debt: no engine/ header carries the NUISystem component vtable; the 111 earlier slots are declared as
//   VirtualSlotNN placeholders so GetSize lands at slot 23 and GetDynamicSize at slot 111.
class C2DVector {                                         // Ego_r.pdb: X, Y (8 bytes)
public:
    float X;
    float Y;
};
namespace NUISystem {
class CSprite {
public:
    virtual void VirtualSlot00();
    virtual void VirtualSlot01();
    virtual void VirtualSlot02();
    virtual void VirtualSlot03();
    virtual void VirtualSlot04();
    virtual void VirtualSlot05();
    virtual void VirtualSlot06();
    virtual void VirtualSlot07();
    virtual void VirtualSlot08();
    virtual void VirtualSlot09();
    virtual void VirtualSlot10();
    virtual void VirtualSlot11();
    virtual void VirtualSlot12();
    virtual void VirtualSlot13();
    virtual void VirtualSlot14();
    virtual void VirtualSlot15();
    virtual void VirtualSlot16();
    virtual void VirtualSlot17();
    virtual void VirtualSlot18();
    virtual void VirtualSlot19();
    virtual void VirtualSlot20();
    virtual void VirtualSlot21();
    virtual void VirtualSlot22();
    virtual C2DVector GetSize() const;                    // slot 23
    virtual void VirtualSlot24();
    virtual void VirtualSlot25();
    virtual void VirtualSlot26();
    virtual void VirtualSlot27();
    virtual void VirtualSlot28();
    virtual void VirtualSlot29();
    virtual void VirtualSlot30();
    virtual void VirtualSlot31();
    virtual void VirtualSlot32();
    virtual void VirtualSlot33();
    virtual void VirtualSlot34();
    virtual void VirtualSlot35();
    virtual void VirtualSlot36();
    virtual void VirtualSlot37();
    virtual void VirtualSlot38();
    virtual void VirtualSlot39();
    virtual void VirtualSlot40();
    virtual void VirtualSlot41();
    virtual void VirtualSlot42();
    virtual void VirtualSlot43();
    virtual void VirtualSlot44();
    virtual void VirtualSlot45();
    virtual void VirtualSlot46();
    virtual void VirtualSlot47();
    virtual void VirtualSlot48();
    virtual void VirtualSlot49();
    virtual void VirtualSlot50();
    virtual void VirtualSlot51();
    virtual void VirtualSlot52();
    virtual void VirtualSlot53();
    virtual void VirtualSlot54();
    virtual void VirtualSlot55();
    virtual void VirtualSlot56();
    virtual void VirtualSlot57();
    virtual void VirtualSlot58();
    virtual void VirtualSlot59();
    virtual void VirtualSlot60();
    virtual void VirtualSlot61();
    virtual void VirtualSlot62();
    virtual void VirtualSlot63();
    virtual void VirtualSlot64();
    virtual void VirtualSlot65();
    virtual void VirtualSlot66();
    virtual void VirtualSlot67();
    virtual void VirtualSlot68();
    virtual void VirtualSlot69();
    virtual void VirtualSlot70();
    virtual void VirtualSlot71();
    virtual void VirtualSlot72();
    virtual void VirtualSlot73();
    virtual void VirtualSlot74();
    virtual void VirtualSlot75();
    virtual void VirtualSlot76();
    virtual void VirtualSlot77();
    virtual void VirtualSlot78();
    virtual void VirtualSlot79();
    virtual void VirtualSlot80();
    virtual void VirtualSlot81();
    virtual void VirtualSlot82();
    virtual void VirtualSlot83();
    virtual void VirtualSlot84();
    virtual void VirtualSlot85();
    virtual void VirtualSlot86();
    virtual void VirtualSlot87();
    virtual void VirtualSlot88();
    virtual void VirtualSlot89();
    virtual void VirtualSlot90();
    virtual void VirtualSlot91();
    virtual void VirtualSlot92();
    virtual void VirtualSlot93();
    virtual void VirtualSlot94();
    virtual void VirtualSlot95();
    virtual void VirtualSlot96();
    virtual void VirtualSlot97();
    virtual void VirtualSlot98();
    virtual void VirtualSlot99();
    virtual void VirtualSlot100();
    virtual void VirtualSlot101();
    virtual void VirtualSlot102();
    virtual void VirtualSlot103();
    virtual void VirtualSlot104();
    virtual void VirtualSlot105();
    virtual void VirtualSlot106();
    virtual void VirtualSlot107();
    virtual void VirtualSlot108();
    virtual void VirtualSlot109();
    virtual void VirtualSlot110();
    virtual C2DVector GetDynamicSize() const;             // slot 111
};
C2DVector CSprite::GetDynamicSize() const
{
    return GetSize();
}
}