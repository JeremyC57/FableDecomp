// CPlayerGui::SetGuildSealLevelUpPos(const C2DVector& pos) -- queue a screen position for the guild seal's level-up effect; a (near) zero position clears the queue instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043A4D0, length 160 bytes, `ret 4`.
// Callees: the STLport vector<C2DVector>::_M_insert_overflow @ 0x004416E0 (manifest
// "VectorReallocateDouble"); the epsilon float variable @ 0x0129BA3C (read from memory, as in 0x00499C50).
// Layout: PDrawGuildSeal (retail +0x1d0, engine/CPlayerGui.h +0x1d8 less 8); the guild seal's position vector at +0x50
// (descriptive).
// Shape: `pos.x*pos.x + pos.y*pos.y > EPS*EPS` with both components loaded first; STLport push_back (null-checked
// placement copy, else _M_insert_overflow with the __true_type tag homed in the dead argument slot) and clear() as
// erase(begin(), end()) with the inlined POD copy loop of the empty tail.
extern float g_fEpsilon;                                    // 0x0129BA3C

class C2DVector { public:
    float X;
    float Y;
};

inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() { } };              // an empty tag with a user constructor (no zero byte store)

struct C2DVectorVector {                                    // STLport vector<C2DVector> (12 bytes)
    C2DVector* _M_start;
    C2DVector* _M_finish;
    C2DVector* _M_end_of_storage;
    void _M_insert_overflow(C2DVector* position, const C2DVector& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x004416E0
    C2DVector* begin() { return _M_start; }
    C2DVector* end() { return _M_finish; }
    void push_back(const C2DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C2DVector(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    static C2DVector* copy(C2DVector* first, C2DVector* last, C2DVector* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    C2DVector* erase(C2DVector* first, C2DVector* last)
    {
        C2DVector* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

namespace NPlayerGui {
class CDrawGuildSeal { public:
    unsigned char   _pad00[0x50];
    C2DVectorVector LevelUpPositions;                       // +0x50
};
}

class CPlayerGui { public:
    unsigned char               _pad000[0x1d0];
    NPlayerGui::CDrawGuildSeal* PDrawGuildSeal;             // +0x1d0
    void SetGuildSealLevelUpPos(const C2DVector& pos);
};

void CPlayerGui::SetGuildSealLevelUpPos(const C2DVector& pos)
{
    float y = pos.Y;
    float x = pos.X;
    if (x * x + y * y > g_fEpsilon * g_fEpsilon)
        PDrawGuildSeal->LevelUpPositions.push_back(pos);
    else
        PDrawGuildSeal->LevelUpPositions.clear();
}