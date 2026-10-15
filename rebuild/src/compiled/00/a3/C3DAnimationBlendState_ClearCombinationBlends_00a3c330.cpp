// C3DAnimationBlendState::ClearCombinationBlends() -- copy-on-write the shared data, then erase every combination blend
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3C330, length 29 bytes (catalogued as the propagated label
// "CTCMeleeTrap::UpdateTrap"; it sits between C3DAnimationBlendState::AddCombinationBlend @ 0x00A3C310 and
// SetSkeletalMorph @ 0x00A3C350 and starts with the class's MakeOwnCopyOfData like every setter there).
// Leaf: ClearCombinationBlends
// Callees: C3DAnimationBlendState::MakeOwnCopyOfData() @ 0x00A3C1A0, vector<CCombinationBlend>::erase(first, last)
// @ 0x00A3D3E0 (the inlined clear() of the vector at CSharedData+0x0c; AddCombinationBlend pushes onto the same vector
// through 0x00A3D380).
// Shape: `this` in esi across MakeOwnCopyOfData, the shared block re-read from Data (+4), the vector's two words pushed
// last-first and its address formed with `add ecx,0Ch`; the 0-argument caller cannot tail-jump into the `ret 8` callee.
struct CCombinationBlend;

struct CRetailBlendVector {                         // VC6 Dinkumware vector: three words
    CCombinationBlend* m_pFirst;
    CCombinationBlend* m_pLast;
    CCombinationBlend* m_pEnd;
    CCombinationBlend* erase(CCombinationBlend* first, CCombinationBlend* last);   // 0x00A3D3E0
    void clear() { erase(m_pFirst, m_pLast); }
};

struct CSharedData {                                // C3DAnimationBlendState::CSharedData (0x20 bytes, counted)
    virtual void v00();
    long               m_refCount;                  // +0x04
    void*              m_pBaseAnimation;            // +0x08 counted pointer
    CRetailBlendVector CombinationBlends;           // +0x0c
    unsigned char      _pad18[8];                   // +0x18
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                              // +0x04 CIVCountedPointer<CSharedData>
    void MakeOwnCopyOfData();                       // 0x00A3C1A0
    void ClearCombinationBlends();
};

void C3DAnimationBlendState::ClearCombinationBlends()
{
    MakeOwnCopyOfData();
    Data->CombinationBlends.clear();
}