// Behaviour test for 00a77540 (catalog CTCBuyableHouse::MovedInNotification): true when the position list holds an equal C3DVector.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct SRandomAccessTag { SRandomAccessTag() {} };

C3DVector* FindPosition(C3DVector* first, C3DVector* last, const C3DVector& value, const SRandomAccessTag&)
{
    for (; first != last; ++first)
        if (first->X == value.X && first->Y == value.Y && first->Z == value.Z)
            return first;
    return last;
}

struct CPositionList_00a77540
{
    C3DVector* m_first;
    C3DVector* m_last;
    bool ContainsPosition(const C3DVector& position) const
    {
        C3DVector* last = m_last;
        return FindPosition(m_first, last, position, SRandomAccessTag()) != last;
    }
};

int main()
{
    C3DVector list[2] = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };
    C3DVector hit = { 4.0f, 5.0f, 6.0f }, miss = { 4.0f, 5.0f, 7.0f };
    CPositionList_00a77540 p; p.m_first = list; p.m_last = list + 2;
    if (!p.ContainsPosition(hit) || p.ContainsPosition(miss)) { std::printf("00a77540_TEST FAIL\n"); return 1; }
    std::printf("00a77540_TEST PASS\n");
    return 0;
}
