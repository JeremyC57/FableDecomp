// vector<pair<unsigned long, CAnimationEntry> >::resize(size_type n) -- resize(n, value_type()): the default pair (key 0, both def strings -1, no component map) built in a 20-byte frame temporary, passed by const reference, then destroyed (its counted component pointer released)
// Leaf: ?resize@CAnimationPairVector@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3C60, length 68 bytes (+ int3 padding). No callers in
// the shipped build.
// Callees: vector<pair<unsigned long, CAnimationEntry> >::resize(size_type, value_type) @ 0x005E39E0 (the
// element-size reciprocal 0x66666667 >> 3 is a divide by 20; manifest
// "vector<CTCInventoryMap::CUIMiniMapMarker>::resize" is propagated). The chunk label
// "CTCCombat::AddMeleeFlourishAbility" is propagated: the body sits between CAnimationSet::Add 0x005E38C0
// and the CAnimationSet sort helpers, and CAnimationSet::Anims is a CVectorMap<unsigned long,
// CAnimationEntry> (the same resize is inlined in CAnimationSet::TransferBinaryIn 0x005DD440).
// Layout = Ego_r.pdb CAnimationEntry (0x10 bytes): BankIndex +0 (left alone by the constructor),
// AnimName +4 / GroupName +8 (CDefString, table position -1), Components +0xc
// (CIVCountedPointer<CAnimComponentMap>: count at +4, virtual destroy at slot 1).
// Shape: the default pair is a temporary bound to the const reference (`lea eax,[esp]`), its scalar
// stores scheduled around the pushes; `this` stays in ecx for the call; the temporary's destructor is
// the inlined counted-pointer release (`test ; je ; dec [ecx+4] ; jne ; call [vt+4]`), written as a
// Release() on the pointee through DecRefToZero(long&). The slot load lands straight in ecx only with a same-TU
// stand-in body for the two-argument resize that MAY store a non-constant pointer into the temporary's component
// field (without a body the temporary's address escapes and the lane loads the slot into eax and copies it to ecx;
// a body that only reads the value lets VC7.1 prove the field is still 0 and drop the release altogether).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CRefCounted {                                        // second base: the count sits at +4 behind the vptr
    long m_count;
};

struct CCountedVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};

class CAnimComponentMap : public CCountedVirtual, public CRefCounted { public:
    void Release()                                          // on the object: `this` stays in ecx for the virtual call
    {
        if (DecRefToZero(m_count))
            Destroy();
    }
};

class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CIVCountedPointer_AnimComponentMap {
    CAnimComponentMap* Object;
    CIVCountedPointer_AnimComponentMap() : Object(0) { }
    ~CIVCountedPointer_AnimComponentMap()
    {
        CAnimComponentMap* object = Object;
        if (object)
            object->Release();
    }
};

class CAnimationEntry { public:
    long                               BankIndex;           // +0x00
    CDefString                         AnimName;            // +0x04
    CDefString                         GroupName;           // +0x08
    CIVCountedPointer_AnimComponentMap Components;          // +0x0c
    CAnimationEntry() { }
};

struct SAnimationPair {                                     // pair<unsigned long, CAnimationEntry>: 20 bytes
    unsigned long   first;
    CAnimationEntry second;
    SAnimationPair() : first(0), second() { }
};

struct CAnimationPairVector {                               // retail std::vector<pair<unsigned long, CAnimationEntry> >
    SAnimationPair* First;
    SAnimationPair* Last;
    SAnimationPair* End;
    void resize(unsigned long n, const SAnimationPair& value);   // 0x005E39E0
    void resize(unsigned long n);
};


#pragma auto_inline(off)
// Same-TU stand-in for vector::resize(n, value) @ 0x005E39E0 (the real body copies `value` into new elements): it may store a
// non-constant pointer into the value's component field, so VC7.1 keeps the release but knows the address goes no further.
__declspec(noinline) void CAnimationPairVector::resize(unsigned long n, const SAnimationPair& value)
{
    if (value.first == n)
        const_cast<SAnimationPair&>(value).second.Components.Object = (CAnimComponentMap*)Last;
}
#pragma auto_inline(on)

void CAnimationPairVector::resize(unsigned long n)
{
    resize(n, SAnimationPair());
}