// vector<pair<unsigned long, CAnimationEntry> >::resize(size_type n) -- resize(n, value_type()): the default pair (key 0, both def strings -1, no component map) built in a 20-byte frame temporary, passed by const reference, then destroyed (its counted component pointer released)
// Grade: DIFFER(78v68) -- the temporary's release: retail `mov ecx,[esp+10h] ; test ecx,ecx ; je ; dec dword ptr [ecx+4] ; jne`, the lane loads the count into edx (`mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`). Tried: `--Object->m_count` in the destructor, a local pointer copy, Release() on the pointee, Release() on a CRefCounted second base (all the register form). Leaf: ?resize@CAnimationPairVector@@QAEXK@Z
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
// Release() on the pointee (a local or member pointer copy decrements through a register instead).
struct CRefCounted {                                        // second base: the count sits at +4 behind the vptr
    long m_count;
};

struct CCountedVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};

struct CAnimComponentMap : CCountedVirtual, CRefCounted {
    void Release()                                          // on the object: `this` stays in ecx for the virtual call
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CIVCountedPointer_AnimComponentMap {
    CAnimComponentMap* Object;
    CIVCountedPointer_AnimComponentMap() : Object(0) { }
    ~CIVCountedPointer_AnimComponentMap()
    {
        if (Object)
            Object->Release();
    }
};

struct CAnimationEntry {
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

void CAnimationPairVector::resize(unsigned long n)
{
    resize(n, SAnimationPair());
}
