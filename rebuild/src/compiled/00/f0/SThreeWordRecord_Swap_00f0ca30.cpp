// SThreeWordRecord::Swap(SThreeWordRecord& other) -- exchange the three words of this record with another one, word by word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0CA30, length 41 bytes (`ret 4`, eax not set: void).
// The manifest label (CNearestBuildingWithProperty::CNearestBuildingWithProperty) is propagated and wrong: the body
// swaps +0, +4, +8 of `this` with the same words of the argument. It sits in the CQ_AmbushTradersScript code next to
// the TeleportData copy constructor @ 0x00F0CA60, so the record is probably a script data triple; named by effect.
// Shape: three inlined std::swap calls, each loading this-word into edx and other-word into esi, storing the
// other-word first.
// Leaf: ?Swap@SThreeWordRecord
struct SThreeWordRecord {
    unsigned long m_a;   // +0x00
    unsigned long m_b;   // +0x04
    unsigned long m_c;   // +0x08
    void Swap(SThreeWordRecord& other);
};

template <class T> inline void SwapValues(T& left, T& right)
{
    T temp = left;
    left = right;
    right = temp;
}

void SThreeWordRecord::Swap(SThreeWordRecord& other)
{
    SwapValues(m_a, other.m_a);
    SwapValues(m_b, other.m_b);
    SwapValues(m_c, other.m_c);
}