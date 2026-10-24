// Behaviour test for 0x00762110 CTCShapeManager::~CTCShapeManager: removes every occurrence of this manager from the
// global vector (STLport remove + single-element erase of the remove result), keeps the others in order, resets the
// selection and destroys the shape list.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CPtrVector {
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    void** erase(void** pos)
    {
        if (pos + 1 != _M_finish)
            std::memmove(pos, pos + 1, (char*)_M_finish - (char*)(pos + 1));
        --_M_finish;
        return pos;
    }
};

static CPtrVector gShapeManagers;
static int gShapeDtors = 0;
static int gFrees = 0;

void** remove_model(void** first, void** last, void* const& val)
{
    while (first != last && *first != val) ++first;
    if (first == last) return first;
    void** result = first;
    for (++first; first != last; ++first)
        if (!(*first == val)) { *result = *first; ++result; }
    return result;
}

struct CShapePtr { void* Data; void* Info; ~CShapePtr() { ++gShapeDtors; } };

struct CTCShapeManager {
    CShapePtr* PersistantShapes_start;
    CShapePtr* PersistantShapes_finish;
    CShapePtr* SelectedShape;
    void Destroy()                                 // the destructor body + member vector destructor
    {
        void* self = this;
        gShapeManagers.erase(remove_model(gShapeManagers._M_start, gShapeManagers._M_finish, self));
        SelectedShape = PersistantShapes_finish;
        for (CShapePtr* p = PersistantShapes_start; p != PersistantShapes_finish; ++p) p->~CShapePtr();
        if (PersistantShapes_start) { std::free(PersistantShapes_start); ++gFrees; }
    }
};

int main()
{
    CTCShapeManager a, b, c;
    a.PersistantShapes_start = (CShapePtr*)std::malloc(3 * sizeof(CShapePtr));
    a.PersistantShapes_finish = a.PersistantShapes_start + 3;
    a.SelectedShape = a.PersistantShapes_start;
    b.PersistantShapes_start = b.PersistantShapes_finish = b.SelectedShape = 0;
    void* storage[8] = { &b, &a, &c, &a, &b, 0, 0, 0 };
    gShapeManagers._M_start = storage; gShapeManagers._M_finish = storage + 5; gShapeManagers._M_end_of_storage = storage + 8;
    int ok = 1;
    a.Destroy();                                   // two occurrences: remove compacts [b, c, b] and erase drops one slot
    if (gShapeManagers._M_finish != storage + 4) ok = 0;
    if (storage[0] != &b || storage[1] != &c || storage[2] != &b) ok = 0;
    if (gShapeDtors != 3 || gFrees != 1 || a.SelectedShape != a.PersistantShapes_finish) ok = 0;
    // reset to a clean registry with b registered once
    void* storage2[4] = { &c, &b, 0, 0 };
    gShapeManagers._M_start = storage2; gShapeManagers._M_finish = storage2 + 2; gShapeManagers._M_end_of_storage = storage2 + 4;
    b.Destroy();                                   // last element: no memmove, finish steps back
    if (gShapeManagers._M_finish != storage2 + 1 || storage2[0] != &c || gFrees != 1) ok = 0;
    if (!ok) { std::printf("00762110_TEST FAIL\n"); return 1; }
    std::printf("00762110_TEST PASS\n");
    return 0;
}