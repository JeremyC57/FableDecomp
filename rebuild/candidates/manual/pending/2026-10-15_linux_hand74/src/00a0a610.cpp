// _unattributed::NamedCountedPointerSizeof() -- memory size of a {CCharString name, CCountedPointer<T> pointer} record: the pointer's 8 bytes plus its object's own Sizeof() when set, plus the name's Sizeof()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0A610, 50 bytes (byte-identical twin at 0x00A0A550: another instantiation).
// Manifest label "CScriptThing::GetCurrentScriptPriority" is propagated and rejected: the body reads a
// CCharString at +0 (passed to CCharString::Sizeof 0x0099E500 -- 4 when empty, else 0x15 + length) and a
// counted pointer at +4 whose object answers a virtual Sizeof (slot 20, +0x50). The record is laid out like
// std::pair<const CCharString, CCountedPointer<T>> (a _Dest_val of pair<const CCharString,
// CCountedPointer<C3DSkeletalMorphTarget>> sits between the twins); no direct caller references either twin,
// so the owning class is not recoverable and the leaf states what the body does.
// Callees: the object's virtual Sizeof (+0x50), CCharString::Sizeof @ 0x0099E500.
//
// Shape: the counted pointer's inline Sizeof() (`Data ? Data->Sizeof() + sizeof(*this) : sizeof(*this)`)
// is evaluated first into esi (`add esi,8` / `mov esi,8`); the name's call and the final `add eax,esi` are
// tail-duplicated into both arms: `return Pointer.Sizeof() + Name.Sizeof();`.
// Leaf: NamedCountedPointerSizeof

struct CCharString {
    void* m_pData;                               // +0x00
    unsigned long Sizeof() const;                // 0x0099E500
};

struct CSizedObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual unsigned long Sizeof() const;        // slot 20 (+0x50)
};

struct CCPPointerInfo;

struct CCountedPointer_CSizedObject {
    CSizedObject* Data;                          // +0x00
    CCPPointerInfo* Info;                        // +0x04
    unsigned long Sizeof() const { return Data ? Data->Sizeof() + sizeof(*this) : sizeof(*this); }
};

struct SNamedCountedPointer {
    CCharString Name;                            // +0x00
    CCountedPointer_CSizedObject Pointer;        // +0x04
    unsigned long NamedCountedPointerSizeof() const;
};

unsigned long SNamedCountedPointer::NamedCountedPointerSizeof() const
{
    return Pointer.Sizeof() + Name.Sizeof();
}
