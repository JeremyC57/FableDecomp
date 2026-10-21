// CAnimSet::Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha) -- reset the set from its init data: one anim and one blend alpha (alpha, alpha), the current world frame, the speed/flag/priority fields and the looping/forwards bits
// Grade: DIFFER(ours v retail) -- 331v331: the __true_type tag temporaries are homed in the live alpha argument slot in retail (ours: the data / animator slots) and the CurrentAlpha/LastAlpha stores are scheduled apart in retail (ours adjacent); 4 forms tried (short field, unsigned short field, STLport __copy_trivial, named tag local)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D1A0, length 329 bytes (`ret 0xc`).
// Callees: memmove @ 0x00BFEAE6 (the inlined vector<short>::clear's trivial copy), vector<short>::_M_insert_overflow
//          @ 0x00710980 and vector<CTransitionBlendAlpha>::_M_insert_overflow @ 0x00710F70 (manifest "Vector_Reallocate_*"),
//          CWorld::GetWorldFrame @ 0x0049D870 (static).
// Layout: Ego_r.pdb CAnimSet with the two 12-byte STLport vectors (PDB offsets 4 high after Anims and 8 high after
//          InternalAlphas): UpdateMode +0, Anims +4, InternalAlphas +0x10, CurrentPercentageComplete +0x1c,
//          LastPercentageComplete +0x20, Speed +0x24, FrameInternalAlphasUpdated +0x28, CurrentAlpha +0x2c, LastAlpha +0x30,
//          LastUpdateChange +0x34, ChangeAlphaBy +0x38, PAnimator +0x3c, Flags +0x40, TypeFlags +0x44, Priority +0x4c,
//          Type +0x50, bits +0x51 (Looping 0, Finished 1, Forwards 2, HadUpdate 3). CTransitionBlendAlpha {CurrentAlpha,
//          LastAlpha}. The init data block (descriptive names): UpdateMode +0, Type +4, Looping +5, Anim +8 (short), Speed
//          +0xc, Alpha +0x10, PercentageComplete +0x18, LastPercentageComplete +0x1c, Flags +0x20, TypeFlags +0x24,
//          Priority +0x28.
// Shape: each vector is `clear()` (inline erase(begin(), end()): memmove-based trivial copy for shorts, an element loop for
//          the 8-byte alphas) then `push_back` (inline fast path, _M_insert_overflow with the __true_type tag otherwise); the
//          short temporary is homed in the dead animator argument slot.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);
    static T* copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    T* erase(T* first, T* last)
    {
        T* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1UL, true);
        }
    }
};
template <> short* vector<short>::copy(short* first, short* last, short* result)   // STLport __copy_trivial
{
    return (last == first) ? result
        : (short*)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
}

struct CTransitionBlendAlpha {
    float CurrentAlpha;
    float LastAlpha;
    CTransitionBlendAlpha(float current, float last) : CurrentAlpha(current), LastAlpha(last) { }
};
struct CWorld {
    static unsigned long GetWorldFrame();          // 0x0049D870
};
struct CAnimateThing;
struct CAnimSetInitData {
    long           UpdateMode;                     // +0x00
    unsigned char  Type;                           // +0x04
    bool           Looping;                        // +0x05
    unsigned char  _pad06[2];
    unsigned short Anim;                           // +0x08 (converted to the short element: a temp)
    float          Speed;                          // +0x0c
    float          Alpha;                          // +0x10
    long           _pad14;
    float          PercentageComplete;             // +0x18
    float          LastPercentageComplete;         // +0x1c
    unsigned long  Flags;                          // +0x20
    unsigned long  TypeFlags;                      // +0x24
    long           Priority;                       // +0x28
};

struct CAnimSet {
    long          UpdateMode;                      // +0x00
    vector<short> Anims;                           // +0x04
    vector<CTransitionBlendAlpha> InternalAlphas;  // +0x10
    float         CurrentPercentageComplete;       // +0x1c
    float         LastPercentageComplete;          // +0x20
    float         Speed;                           // +0x24
    unsigned long FrameInternalAlphasUpdated;      // +0x28
    float         CurrentAlpha;                    // +0x2c
    float         LastAlpha;                       // +0x30
    float         LastUpdateChange;                // +0x34
    float         ChangeAlphaBy;                   // +0x38
    CAnimateThing* PAnimator;                      // +0x3c
    unsigned long Flags;                           // +0x40
    unsigned long TypeFlags;                       // +0x44
    long          AnimDelay;                       // +0x48
    long          Priority;                        // +0x4c
    unsigned char Type;                            // +0x50
    bool          Looping : 1;                     // +0x51
    bool          Finished : 1;
    bool          Forwards : 1;
    bool          HadUpdate : 1;
    bool          OwnedByMainAnim : 1;
    void Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha);
};

void CAnimSet::Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha)
{
    PAnimator = animator;
    Anims.clear();
    Anims.push_back(data.Anim);
    InternalAlphas.clear();
    InternalAlphas.push_back(CTransitionBlendAlpha(data.Alpha, data.Alpha));
    FrameInternalAlphasUpdated = CWorld::GetWorldFrame();
    CurrentPercentageComplete = data.PercentageComplete;
    LastPercentageComplete = data.LastPercentageComplete;
    CurrentAlpha = alpha;
    LastUpdateChange = 0.0f;
    ChangeAlphaBy = 0.0f;
    LastAlpha = CurrentAlpha;
    Type = data.Type;
    UpdateMode = data.UpdateMode;
    Looping = data.Looping;
    Speed = data.Speed;
    Finished = false;
    Forwards = true;
    Flags = data.Flags;
    TypeFlags = data.TypeFlags;
    HadUpdate = false;
    Priority = data.Priority;
}
