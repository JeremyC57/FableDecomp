#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailList_004638a1::Insert(SIterator where, SIterator first, SIterator last) -- the range insert forwarder: the three 4-byte iterator classes by value and a zero-initialised one-byte tag local (`stosb`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004638A1, length 39 bytes.
// Callees: the range insert @ 0x004638C8 (on the list: the position iterator by value -- its copy constructed in place in the reserved slot (`push ecx ; mov eax,esp ;
// mov [eax],edx`) -- the two range words and the tag's address). Shape: a frame pointer (the game module), the tag local in the one-word frame (`push ecx`), edi saved for the `stosb`, `ret 0Ch`. The chunk label
// "CEngineSceneGrid::GetCellBoundingBox" is propagated across the family; named by effect.
struct STag_004638a1 { unsigned char m_tag; };   // POD: `STag()` value-initialises it (the `xor eax,eax ; stosb` into the frame byte)

struct SIterator_004638a1 {                           // a 4-byte iterator class with a user copy constructor (passed by value through a reserved slot)
    void* m_pNode;
    SIterator_004638a1(const SIterator_004638a1& other) : m_pNode(other.m_pNode) { }
};

struct CRetailList_004638a1 {
    void* m_pHead;
    void InsertRange(SIterator_004638a1 where, void* first, void* last, const STag_004638a1& tag);   // 0x004638C8
    void Insert(SIterator_004638a1 where, void* first, void* last);
};

void CRetailList_004638a1::Insert(SIterator_004638a1 where, void* first, void* last)
{
    InsertRange(where, first, last, STag_004638a1());
}