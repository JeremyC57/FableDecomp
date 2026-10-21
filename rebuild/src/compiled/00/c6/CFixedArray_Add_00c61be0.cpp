// CFixedArray::Add(long) -- appends a value to a preallocated array (no bounds check)
// Leaf: Add@CFixedArray
// Retail: Fable.exe @ 0x00C61BE0, 21 bytes, `ret 4`. Catalog label CCreatureActionBase::GetFramesRemaining is wrong: the body
// stores its argument at Data[Count] (+0x04/+0x08) and increments Count, returning nothing. Only caller 0x00C44B69.
// The name here is behavioural.
class CFixedArray {
public:
    void* _vptr_pad;                                     // +0x00
    long* Data;                                          // +0x04
    long  Count;                                         // +0x08
    void Add(long value);
};
void CFixedArray::Add(long value)
{
    Data[Count] = value;
    ++Count;
}