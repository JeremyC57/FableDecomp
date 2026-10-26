// Behaviour test for 0x00762090 CTCShapeManager::CTCShapeManager: the component starts with an empty shape list,
// cleared flags, the selection at end(), and is appended to the global shape-manager vector (in place or by growth).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CThing { public: int id; };

struct CPtrVector {                               // STLport vector<CTCShapeManager*> shape
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    int m_overflows;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(void** pos, void* const& x, unsigned int fill_len, bool atend)
    {
        ++m_overflows;
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        void** s = (void**)std::malloc(len * sizeof(void*));
        unsigned int head = (unsigned int)(pos - _M_start);
        if (head) std::memmove(s, _M_start, head * sizeof(void*));
        void** f = s + head;
        for (unsigned int n = fill_len; n > 0; --n) *f++ = x;
        if (!atend) { unsigned int tail = (unsigned int)(_M_finish - pos); std::memmove(f, pos, tail * sizeof(void*)); f += tail; }
        std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void push_back(void* const& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) void*(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

static CPtrVector gShapeManagers = { 0, 0, 0, 0 };

struct CShapePtr { void* Data; void* Info; };

class CTCShapeManager { public:
    CThing* ParentThing;
    CShapePtr* PersistantShapes_start;
    CShapePtr* PersistantShapes_finish;
    CShapePtr* PersistantShapes_end;
    CShapePtr* SelectedShape;
    bool Activated, NewShapeActivated, LeavingShape;
    explicit CTCShapeManager(CThing& thing)
        : ParentThing(&thing), PersistantShapes_start(0), PersistantShapes_finish(0), PersistantShapes_end(0),
          Activated(false), NewShapeActivated(false), LeavingShape(false)
    {
        void* self = this;
        gShapeManagers.push_back(self);
        SelectedShape = PersistantShapes_finish;
    }
};

int main()
{
    CThing thing = { 7 };
    int ok = 1;
    unsigned char raw[5][sizeof(CTCShapeManager)];
    CTCShapeManager* m[5];
    for (int i = 0; i < 5; ++i) {
        std::memset(raw[i], 0xCD, sizeof(raw[i]));
        m[i] = new (raw[i]) CTCShapeManager(thing);
    }
    for (int i = 0; i < 5; ++i) {
        if (m[i]->ParentThing != &thing || m[i]->Activated || m[i]->NewShapeActivated || m[i]->LeavingShape) ok = 0;
        if (m[i]->PersistantShapes_start || m[i]->PersistantShapes_finish || m[i]->PersistantShapes_end) ok = 0;
        if (m[i]->SelectedShape != m[i]->PersistantShapes_finish) ok = 0;
        if (gShapeManagers._M_start[i] != m[i]) ok = 0;
    }
    if (gShapeManagers.size() != 5u || gShapeManagers.m_overflows != 4) ok = 0;   // capacities 1, 2, 4, 8
    std::free(gShapeManagers._M_start);
    if (!ok) { std::printf("00762090_TEST FAIL\n"); return 1; }
    std::printf("00762090_TEST PASS\n");
    return 0;
}