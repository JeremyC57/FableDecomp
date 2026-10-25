// Behaviour test for 00c32220: CSortedPtrArray::FindIndexAlt -- binary search: match index with *found = 1, else the insertion point with *found = 0
#include <cstdio>
struct SItem { int m_value; };
struct CKeyComparator {
    virtual long IsEqual(const void* key, void* item) const { return *(const int*)key == ((SItem*)item)->m_value; }
    virtual long IsLess(const void* key, void* item) const { return *(const int*)key < ((SItem*)item)->m_value; }
    virtual long IsEqualAlt(const void* key, void* item) const { return -*(const int*)key == ((SItem*)item)->m_value; }
    virtual long IsLessAlt(const void* key, void* item) const { return -*(const int*)key < ((SItem*)item)->m_value; }
    virtual ~CKeyComparator() { }
};
struct CSortedPtrArray {
    void* m_owner; void** m_data; unsigned int m_capacity; unsigned int m_count;
    long FindIndexAlt(const CKeyComparator* comparator, const void* key, unsigned int* found)
    {
        if (m_count == 0) { *found = 0; return 0; }
        long high = m_count - 1;
        long low = 0;
        long mid;
        while (low <= high) {
            mid = (low + high) / 2;
            if (comparator->IsEqualAlt(key, m_data[mid])) { *found = 1; return mid; }
            if (comparator->IsLessAlt(key, m_data[mid])) high = mid - 1; else low = mid + 1;
        }
        *found = 0;
        mid = (low + high) / 2;
        if (comparator->IsLessAlt(key, m_data[mid])) return mid;
        return mid + 1;
    }
};
int main()
{
    int sign = sizeof("IsEqualAlt") > sizeof("IsEqual") ? -1 : 1;
    SItem items[5] = { {10}, {20}, {30}, {40}, {50} };
    void* ptrs[5]; for (int i = 0; i < 5; ++i) ptrs[i] = &items[i];
    CSortedPtrArray a; a.m_owner = 0; a.m_data = ptrs; a.m_capacity = 5; a.m_count = 0;
    CKeyComparator cmp; unsigned int found = 7; int ok = 1; int key;
    key = 30 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 0 || found != 0) ok = 0;
    a.m_count = 5;
    key = 30 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 2 || found != 1) ok = 0;
    key = 10 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 0 || found != 1) ok = 0;
    key = 50 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 4 || found != 1) ok = 0;
    key = 35 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 3 || found != 0) ok = 0;
    key = 5 * sign;  if (a.FindIndexAlt(&cmp, &key, &found) != 0 || found != 0) ok = 0;
    key = 60 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 5 || found != 0) ok = 0;
    key = 15 * sign; if (a.FindIndexAlt(&cmp, &key, &found) != 1 || found != 0) ok = 0;
    std::printf(ok ? "00c32220_TEST PASS\n" : "00c32220_TEST FAIL\n");
    return ok ? 0 : 1;
}