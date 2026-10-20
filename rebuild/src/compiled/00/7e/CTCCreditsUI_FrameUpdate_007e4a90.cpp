// CTCCreditsUI::FrameUpdate() -- a quitting credits screen deletes its UI components and switches itself off; an active one quits once the credits component (looked up by the current part name) reports state 3
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E4A90, 193 bytes (plain `ret`). CTCCreditsUI vtable 0x012705AC slot 9 (the
// catalog's "vfunc_9"; slot 9 is FrameUpdate, as CTCChest 0x00750780).
// Callees: each component's virtual scalar deleting dtor (slot 0, `push 1`), memmove @ 0x00BFEAE6 (the inline erase),
// CCharString(const CCharString&) @ 0x0099EC30 (the by-value argument built in place), the first component's virtual slot 3
// (+0x0c; child lookup by name, named descriptively) and the found component's virtual slot 86 (+0x158; its state).
// Layout: as the landed DrawPreparePrimitivesForRendering 0x007E49D0: Active +0x0c, Components (STLport vector<NUISystem::
// CComponent*>) +0x10, Quitting +0x1c, CreditPart +0x20.
// Shape: `delete Components[i]` per index with the size re-divided every pass; clear() is the inline erase(begin(), end())
// whose trivial copy is `if (src == srcEnd) return dest; memmove(...) + bytes` (the `cmp eax,eax` early-out); the lookup's
// vtable is read before the by-value string is built in the argument slot.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
};
namespace NUISystem {
struct CComponent {
    virtual ~CComponent();                              // slot 0 (scalar deleting dtor)
    virtual void v01(); virtual void v02();
    virtual CComponent* GetChild(CCharString name);     // slot 3 (+0x0c)
#define V(n) virtual void v##n();
    V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23)
    V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43)
    V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63)
    V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83)
    V(84) V(85)
#undef V
    virtual long GetState();                            // slot 86 (+0x158)
};
}
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    T& front() const { return *_M_start; }
    static T* CopyTrivial(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned int bytes = (unsigned int)((char*)srcEnd - (char*)src);
        return (T*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void erase(T* first, T* last) { _M_finish = CopyTrivial(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
};
struct CTCBase {
    virtual void v00();
    struct CThing* ParentThing;                         // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
struct CTCCreditsUI : CTCBase {
    bool Active;                                        // +0x0c
    CRetailVector<NUISystem::CComponent*> Components;   // +0x10
    bool Quitting;                                      // +0x1c
    CCharString CreditPart;                             // +0x20
    void FrameUpdate();
};

void CTCCreditsUI::FrameUpdate()
{
    if (Quitting) {
        for (unsigned int i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
        Active = false;
        Quitting = false;
        return;
    }
    if (Active && !Components.empty()) {
        NUISystem::CComponent* child = Components.front()->GetChild(CreditPart);
        if (child && child->GetState() == 3 && Active)
            Quitting = true;
    }
}