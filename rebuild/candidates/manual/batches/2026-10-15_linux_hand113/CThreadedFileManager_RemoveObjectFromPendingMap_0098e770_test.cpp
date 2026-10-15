#include <cstdio>
// Behaviour test for CThreadedFileManager::RemoveObjectFromPendingMap (0x0098E770): the object is erased from
// DeviceQueue[device][priority]; the queue cursor steps on only when it pointed at the erased object.
struct CThreadedFile { long DeviceID; };
struct CFileObject { long Priority; CThreadedFile* File; };
struct CPendingObjectSortKey { CFileObject* Object; CPendingObjectSortKey(CFileObject* o) : Object(o) { } };
struct CPendingMap {                       // tiny ordered set of objects, enough for the test
    CFileObject* items[4]; int count;
    int find(const CPendingObjectSortKey& k) { for (int i = 0; i < count; ++i) if (items[i] == k.Object) return i; return count; }
    unsigned long erase(const CPendingObjectSortKey& k)
    {
        int i = find(k); if (i == count) return 0;
        for (; i + 1 < count; ++i) items[i] = items[i + 1];
        --count; return 1;
    }
};
struct CPendingIterator { int m_node; CPendingIterator& operator++() { ++m_node; return *this; } };
struct CPendingObjectMap { CPendingMap Objects; CPendingIterator NextObject; };
struct CThreadedFileManager {
    CPendingObjectMap DeviceQueue[2][7];
    void RemoveObjectFromPendingMap(CFileObject* object)
    {
        long device = object->File->DeviceID;
        CPendingObjectSortKey key(object);
        CPendingIterator& next = DeviceQueue[device][object->Priority].NextObject;
        if (next.m_node == DeviceQueue[device][object->Priority].Objects.find(key))
            ++next;
        DeviceQueue[device][object->Priority].Objects.erase(key);
    }
};
int main()
{
    static CThreadedFileManager m;
    CThreadedFile f; f.DeviceID = 1;
    CFileObject a, b; a.Priority = 3; a.File = &f; b.Priority = 3; b.File = &f;
    CPendingObjectMap& q = m.DeviceQueue[1][3];
    q.Objects.items[0] = &a; q.Objects.items[1] = &b; q.Objects.count = 2; q.NextObject.m_node = 0;
    m.RemoveObjectFromPendingMap(&a);          // cursor at a: steps on
    if (q.Objects.count != 1 || q.Objects.items[0] != &b || q.NextObject.m_node != 1) { std::printf("0098e770_TEST FAIL\n"); return 1; }
    q.NextObject.m_node = 5;
    m.RemoveObjectFromPendingMap(&b);          // cursor elsewhere: untouched
    if (q.Objects.count != 0 || q.NextObject.m_node != 5 || m.DeviceQueue[0][3].Objects.count != 0) { std::printf("0098e770_TEST FAIL\n"); return 1; }
    std::printf("0098e770_TEST PASS\n");
    return 0;
}
