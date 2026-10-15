// Behaviour test for CNavSwitchableLeafNode::IsEveryUniqueIDOpen (0x00A773E0): true for an empty map or when every
// entry is open, false as soon as one entry is closed.
#include <cstdio>
struct SEntry { unsigned long idLo, idHi; bool Open; };
struct CMap { SEntry* m_pFirst; SEntry* m_pLast; SEntry* m_pEnd; unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); } const SEntry& operator[](unsigned int i) const { return m_pFirst[i]; } };
struct CNode {
    CMap UniqueIDsOpen;
    bool IsEveryUniqueIDOpen() const
    {
        for (unsigned int i = 0; i < UniqueIDsOpen.size(); ++i) {
            if (!UniqueIDsOpen[i].Open)
                return false;
        }
        return true;
    }
};
int main()
{
    SEntry e[3]; for (int k = 0; k < 3; ++k) { e[k].idLo = k; e[k].idHi = 0; e[k].Open = true; }
    CNode n; n.UniqueIDsOpen.m_pFirst = e; n.UniqueIDsOpen.m_pLast = e + 3; n.UniqueIDsOpen.m_pEnd = e + 3;
    bool ok = n.IsEveryUniqueIDOpen();
    e[2].Open = false; ok = ok && !n.IsEveryUniqueIDOpen();
    e[2].Open = true; e[0].Open = false; ok = ok && !n.IsEveryUniqueIDOpen();
    n.UniqueIDsOpen.m_pLast = e; ok = ok && n.IsEveryUniqueIDOpen();
    if (!ok) { std::printf("00a773e0_TEST FAIL\n"); return 1; }
    std::printf("00a773e0_TEST PASS\n");
    return 0;
}
