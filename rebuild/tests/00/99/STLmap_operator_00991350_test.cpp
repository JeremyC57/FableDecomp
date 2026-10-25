// Behaviour test for STLport map<CPendingObjectSortKey, CFileObject*>::operator[] (0x00991350): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key (sort key, file position, object address).
#include <cstdio>
#include <cstring>

struct CThreadedFile { unsigned int PhysicalSortKey; };
struct CFileObject { unsigned int FilePos; CThreadedFile* PFile; };
struct K {
    CFileObject* Object;
};
inline bool Less(const K& a, const K& b)            // physical sort key, then file position, then address
{
    unsigned int key = a.Object->PFile->PhysicalSortKey;
    unsigned int otherKey = b.Object->PFile->PhysicalSortKey;
    if (key < otherKey) return true;
    if (key > otherKey) return false;
    if (a.Object->FilePos < b.Object->FilePos) return true;
    if (a.Object->FilePos > b.Object->FilePos) return false;
    return a.Object < b.Object;
}
typedef CFileObject* T;

struct Node { Node* next; K first; T second; };

struct Map {
    Node* head;                                     // sorted singly linked list stands in for the tree
    unsigned int count;
    Map() : head(0), count(0) {}
    ~Map() { while (head) { Node* n = head->next; delete head; head = n; } }
    Node** lower_bound(const K& k)
    {
        Node** p = &head;
        while (*p && Less((*p)->first, k)) p = &(*p)->next;
        return p;
    }
    T& operator[](const K& k)                       // STLport shape
    {
        Node** i = lower_bound(k);
        if (*i == 0 || Less(k, (*i)->first)) {
            Node* n = new Node;
            n->first = k; n->second = T(); n->next = *i; *i = n; ++count;
        }
        return (*i)->second;
    }
};

#define CHECK(c) if (!(c)) { std::printf("00991350_TEST FAIL\n"); return 1; }

int main()
{
    CThreadedFile f1, f2; f1.PhysicalSortKey = 10; f2.PhysicalSortKey = 20;
    CFileObject objs[3];
    objs[0].PFile = &f2; objs[0].FilePos = 0;
    objs[1].PFile = &f1; objs[1].FilePos = 100;
    objs[2].PFile = &f1; objs[2].FilePos = 50;
    Map m;
    for (int i = 0; i < 3; ++i) {
        K k; k.Object = &objs[i];
        CHECK(m[k] == 0);
        m[k] = &objs[i];
    }
    CHECK(m.count == 3);
    CHECK(m.head->second == &objs[2] && m.head->next->second == &objs[1] && m.head->next->next->second == &objs[0]);
    K again; again.Object = &objs[1];
    CHECK(m[again] == &objs[1] && m.count == 3);
    std::printf("00991350_TEST PASS\n");
    return 0;
}