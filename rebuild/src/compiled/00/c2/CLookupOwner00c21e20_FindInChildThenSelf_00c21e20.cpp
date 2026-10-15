// CLookupOwner_00c21e20::FindInChildThenSelf(long key) -- asks the embedded child node (virtual slot 17) for `key` and, when it has nothing, falls back to the owner's own slot-17 lookup
// Leaf: FindInChildThenSelf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21E20, length 43 bytes (+5 int3 padding). The catalog label
// "CTCInventoryBase::ProcessButtonAReleased" is propagated and wrong: the callers (0x00C11369.. in the 0x00C11330 /
// 0x00C113B0 spell/weapon selection helpers) pass `lea ecx,[esi+2Ch]` and call slot 1/2 of the returned object, so this
// is a lookup on an unnamed polymorphic owner. Class and member names are descriptive.
// Callees: virtual slot 17 (+0x44) of the member object at +0xD8 and of `this`, both taking the key by reference
// (`lea edx,[esp+8] ; push edx`: the address of the by-value argument).
// Shape: speed-optimised (no pragma); the result is a pointer local tested once (`test eax,eax ; jne`).
struct CObject;
struct CLookupNode_00c21e20 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual CObject* Find(const long& key);                      // slot 17 (+0x44)
};
struct CLookupOwner_00c21e20 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual CObject* Find(const long& key);                      // slot 17 (+0x44)
    unsigned char _pad_0x04[0xD4];
    CLookupNode_00c21e20   m_child;                                       // +0xD8
    CObject* FindInChildThenSelf(long key);
};
CObject* CLookupOwner_00c21e20::FindInChildThenSelf(long key)
{
    CObject* found = m_child.Find(key);
    if (found == 0)
        found = Find(key);
    return found;
}