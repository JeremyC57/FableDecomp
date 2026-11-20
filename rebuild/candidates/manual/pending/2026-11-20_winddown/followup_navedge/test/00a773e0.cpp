// Behaviour test for 0x00A773E0 CNavSwitchableLeafNode::IsEveryUniqueIDOpen (links the REAL row object): true for an empty map or
// when every entry is open, false as soon as one entry is closed (first, middle or last).
// The member is private (Ego_r.pdb ...@@ABE_NXZ; the access is part of the decorated name), so the test declares the class with main
// as a friend instead of including the shared header: only the member's name and the map's two words at +0x20 / +0x24 matter here.
class CNavSwitchableLeafNode {
private:
    bool IsEveryUniqueIDOpen() const;                      // 0x00A773E0 (the row object)
    friend int main();
};
#include <stdio.h>
#include <string.h>
struct SEntry { unsigned __int64 first; bool second; unsigned char pad[7]; };   // std::pair<unsigned __int64, bool>, 16 bytes
int main()
{
    static unsigned char storage[0x30];                    // a node's bytes; the row reads only the map's start +0x20 / finish +0x24
    SEntry e[4]; memset(e, 0, sizeof(e));
    for (int k = 0; k < 4; ++k) { e[k].first = 0x100000000ULL * k + 7; e[k].second = true; }
    SEntry** words = (SEntry**)(storage + 0x20);
    const CNavSwitchableLeafNode* node = (const CNavSwitchableLeafNode*)(void*)storage;
    int fail = 0;
    words[0] = e; words[1] = e + 4; words[2] = e + 4;
    if (!node->IsEveryUniqueIDOpen()) ++fail;                                     // all open
    for (int k = 0; k < 4; ++k) {                                                 // one shut, at each position
        e[k].second = false; if (node->IsEveryUniqueIDOpen()) ++fail; e[k].second = true;
    }
    e[3].second = false; words[1] = e + 3; if (!node->IsEveryUniqueIDOpen()) ++fail;   // the shut entry is past the end
    words[1] = e + 4;   if (node->IsEveryUniqueIDOpen()) ++fail;
    e[3].second = true; e[0].first = 0; e[0].second = true; if (!node->IsEveryUniqueIDOpen()) ++fail;   // the key is not read
    words[1] = e;       if (!node->IsEveryUniqueIDOpen()) ++fail;                 // empty map
    e[0].second = false; if (!node->IsEveryUniqueIDOpen()) ++fail;                // still empty
    if (sizeof(SEntry) != 16) ++fail;
    if (fail) { printf("00a773e0_TEST FAIL (%d)\n", fail); return 1; }
    printf("00a773e0_TEST PASS\n");
    return 0;
}
