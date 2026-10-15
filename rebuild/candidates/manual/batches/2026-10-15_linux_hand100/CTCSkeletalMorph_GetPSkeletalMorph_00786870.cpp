// CTCSkeletalMorph::GetPSkeletalMorph() -- returns the current skeletal morph (counted pointer at +0x1c) by value, first recalculating it when the morph is marked dirty (+0xc)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786870, length 44 bytes (+4 int3 padding).
// Callee: CTCSkeletalMorph::CalculateSkeletalMorph @ 0x00786700.
// Shape: the result comes back through the hidden pointer (`ret 4`, the result pointer read after the dirty block); the
// counted pointer is {object, count*} with an inline copy constructor that copies both words and bumps the shared count
// when there is one (`inc dword ptr [ecx]`); `pop esi` is scheduled into the null test.
struct CSkeletalMorph;

struct CCountedPointer_CSkeletalMorph {
    CSkeletalMorph* m_pObject;
    long*           m_pCount;
    CCountedPointer_CSkeletalMorph(const CCountedPointer_CSkeletalMorph& other)
        : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++*m_pCount;
    }
};

struct CTCSkeletalMorph {
    unsigned char                  _pad[0xc];
    bool                           m_dirty;            // +0x0c
    unsigned char                  _pad0d[0xf];
    CCountedPointer_CSkeletalMorph m_pSkeletalMorph;   // +0x1c
    void CalculateSkeletalMorph();                     // 0x00786700
    CCountedPointer_CSkeletalMorph GetPSkeletalMorph();
};

CCountedPointer_CSkeletalMorph CTCSkeletalMorph::GetPSkeletalMorph()
{
    if (m_dirty) {
        CalculateSkeletalMorph();
        m_dirty = false;
    }
    return m_pSkeletalMorph;
}
