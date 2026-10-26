// CCombatSequenceBase::PushAction(CCombatAction& action) -- append the action (an 8-byte counted pointer) to CombatActions and count it in NoAvailableActions
// Leaf: ?PushAction@CCombatSequenceBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0B10, 78 bytes, thiscall, `ret 4`.
// Identity: the manifest's "std::vector::push_back" is the inlined body. Ego_r.pdb names
//   ?PushAction@CCombatSequenceBase@@IAEXAAVCCombatAction@@@Z; the callers are the CCombatSequence_* OnCreate bodies
//   (0x00906050 GenericRangedAttack, the landed 0x00907D00 TrollRockAdvance / 0x00909E30 / 0x00906C60 ...).
//   Ego_r.pdb CCombatSequenceBase: Running +4, NoAvailableActions +8, CurrentActionIndex +0xC, CombatActions +0x10
//   (vector<CCombatAction>; 12 bytes in retail), Creature +0x1C, Target +0x20, CreateCalled +0x24; CCombatAction is
//   { CCountedPointer<CCombatActionBase> CreatureAction; } (8 bytes).
// Callees: vector<CCombatAction>::_M_insert_overflow(pos, x, __false_type, 1, true) 0x008B3350.
// Shape: STLport 4.6 push_back inline (`if (finish != end_of_storage) { _Construct(finish, x); ++finish; } else
//   _M_insert_overflow(...)`), then `++NoAvailableActions`. CCombatAction's implicit copy constructor runs the
//   counted pointer's inline one (copy both words, bump Info->RefCount when Info is set); the vector's address is
//   formed before the capacity test; the overflow has a same-TU noinline stand-in so the tag homes in x's slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CThingCreatureBase;
class CCombatActionBase;

class CCPPointerInfo { public:
    long  RefCount;                                         // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CCombatAction {
public:
    CCountedPointer<CCombatActionBase> CreatureAction;      // +0x00
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend);   // 0x008B3350
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line overflow 0x008B3350 (its own row): never reads the tag.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (&__x != 0);
}
#pragma auto_inline(on)

} // namespace std

class CCombatSequenceBase {
public:
    virtual ~CCombatSequenceBase();
    bool                          Running;                  // +0x04
    long                          NoAvailableActions;       // +0x08
    long                          CurrentActionIndex;       // +0x0C
    std::vector<CCombatAction>   CombatActions;            // +0x10
    CThingCreatureBase*           Creature;                 // +0x1C
    CThingCreatureBase*           Target;                   // +0x20
    bool                          CreateCalled;             // +0x24
protected:
    void PushAction(CCombatAction& action);
};

void CCombatSequenceBase::PushAction(CCombatAction& action)
{
    CombatActions.push_back(action);
    ++NoAvailableActions;
}