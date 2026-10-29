// CTCCreditsUI::Activate(CCharString part) -- an inactive credits screen remembers the part, has the front-end manager create its component, resets it into state 1 and keeps it; the screen is then active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E4B60, 165 bytes (`ret 4`; the by-value string is destroyed on both paths).
// Callees: ~CCharString @ 0x0099EAE0, CCharString::operator=(const CCharString&) @ 0x0099EFB0, NUISystem::CFrontEndManager::
// GetInstance() @ 0x0041E5F2 (`__fastcall`, no arguments), NUISystem::CManager::CreateComponent(const CCharString&, bool)
// @ 0x0041DB1D, the component's virtual slot 43 (+0xac; a reset hook, named descriptively) and slot 48 ChangeState(long)
// (+0xc0), the Components vector's _M_insert_overflow @ 0x00485DBC (manifest "Std_Vector_Insert_Range_Fill").
// Layout: as FrameUpdate 0x007E4A90: Active +0x0c, Components +0x10, CreditPart +0x20.
// Shape: the early return destroys the parameter inline; CreateComponent's arguments are pushed before GetInstance runs; the
// component pointer is a named local (the `push ecx` word) whose address the inline push_back hands to _M_insert_overflow
// (with the __true_type tag in the dead parameter word); Active is set after the push_back on both arms. The component stays
// in esi across the virtual calls only when VC7.1 can see that _M_insert_overflow does not let `x` escape: retail had that
// knowledge (link-time code generation), so a stand-in body is compiled above under `auto_inline(off)` (GOTCHAS wave 25 /
// chunk 180; the lander grades only the Activate symbol).
class CCharString { public:
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NUISystem {
class CComponent { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42)
    virtual void Reset();                               // slot 43 (+0xac)
    V(44) V(45) V(46) V(47)
#undef V
    virtual void ChangeState(long state);               // slot 48 (+0xc0)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
struct __true_type { __true_type() { } };
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00485DBC
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T>
void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    T* newStart = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* newFinish = newStart;
    for (T* p = _M_start; p != position; ++p, ++newFinish)
        *newFinish = *p;
    for (unsigned long i = 0; i < fill_len; ++i, ++newFinish)
        *newFinish = x;
    if (!atend)
        for (T* p = position; p != _M_finish; ++p, ++newFinish)
            *newFinish = *p;
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
template struct CRetailVector<NUISystem::CComponent*>;
#pragma auto_inline(on)
class CTCBase { public:
    virtual void v00();
    class CThing* ParentThing;                         // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
class CTCCreditsUI : public CTCBase { public:
    bool Active;                                        // +0x0c
    CRetailVector<NUISystem::CComponent*> Components;   // +0x10
    bool Quitting;                                      // +0x1c
    CCharString CreditPart;                             // +0x20
    void Activate(CCharString part);
};

void CTCCreditsUI::Activate(CCharString part)
{
    if (Active)
        return;
    CreditPart = part;
    NUISystem::CComponent* component = NUISystem::CManager::GetInstance()->CreateComponent(CreditPart, false);
    component->Reset();
    component->ChangeState(1);
    Components.push_back(component);
    Active = true;
}