// Behaviour test for CTCCreditsUI::FrameUpdate (0x007E4A90): quitting deletes every component, empties the list and clears
// Active/Quitting; an active screen starts quitting when the first component's child named CreditPart is in state 3.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* m_pData;
    CCharString(const char* t) : m_pData(t) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { }
};
static int g_deleted = 0; static const char* g_asked = 0;
namespace NUISystem {
struct CComponent {
    long State; CComponent* Child;
    CComponent() : State(0), Child(0) { }
    virtual ~CComponent() { ++g_deleted; }
    virtual CComponent* GetChild(CCharString name) { g_asked = name.m_pData; return Child; }
    virtual long GetState() { return State; }
};
}
template <class T> struct CRetailVector {
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
        return (T*)((char*)std::memmove(dest, src, bytes) + bytes);
    }
    void erase(T* first, T* last) { _M_finish = CopyTrivial(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
};
struct CTCBase {
    virtual void v00() { }
    void* ParentThing;
};
struct CTCCreditsUI : CTCBase {
    bool Active;
    CRetailVector<NUISystem::CComponent*> Components;
    bool Quitting;
    CCharString CreditPart;
    CTCCreditsUI() : CreditPart("CREDITS_PART_1") { }
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

int main()
{
    int bad = 0;
    NUISystem::CComponent* store[3];
    store[0] = new NUISystem::CComponent; store[1] = 0; store[2] = new NUISystem::CComponent;
    NUISystem::CComponent child; child.State = 1;
    store[0]->Child = &child;
    CTCCreditsUI ui; ui.Active = true; ui.Quitting = false;
    ui.Components._M_start = store; ui.Components._M_finish = store + 3; ui.Components._M_end_of_storage = store + 3;
    ui.FrameUpdate();                                // child state 1: keep running
    if (ui.Quitting || std::strcmp(g_asked, "CREDITS_PART_1")) bad |= 1;
    child.State = 3;
    ui.FrameUpdate();                                // state 3: start quitting
    if (!ui.Quitting || !ui.Active) bad |= 2;
    ui.FrameUpdate();                                // quitting: delete both live components, clear
    if (g_deleted != 2 || !ui.Components.empty() || ui.Components._M_finish != store || ui.Active || ui.Quitting) bad |= 4;
    ui.Active = true;
    ui.FrameUpdate();                                // empty list: nothing
    if (ui.Quitting) bad |= 8;
    if (bad) { std::printf("007e4a90_TEST FAIL\n"); return 1; }
    std::printf("007e4a90_TEST PASS\n");
    return 0;
}
