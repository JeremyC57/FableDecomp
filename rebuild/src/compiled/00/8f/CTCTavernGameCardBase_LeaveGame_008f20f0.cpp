// CTCTavernGameCardBase::LeaveGame() -- tavern-game LeaveGame, then kill every live pointer particle effect and empty the effect list, empty the card list, kill and free every card left in the pack, forget the selected/highlighted cards and hand the camera back
// Leaf: ?LeaveGame@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F20F0, 387 bytes (plain `ret`); CTCTavernGameCardBase vtable 0x01288294
// slot 30 (the LeaveGame slot; CardSorting 0x007D4DE0 and CardPairs 0x007D5F60, landed, call this body first).
// Callees: CTCTavernGame::LeaveGame() 0x005EC650 (non-virtual base call); CBaseIntelligentPointer::GetPItem 0x00A01B50,
//          ::SetPItem 0x00A01B90; CThing::Kill(bool) 0x004C9B80; memmove 0x00BFEAE6 (STLport __copy_trivial of the
//          card list's clear()); CTCTavernGame::KillThing(CThing*) 0x005E9A60; operator delete 0x00BFE9BC;
//          CTCBase::GetWorld 0x00686A70 (manifest "CViewBase::GetDisplayEngine" is propagated); the world's +0x34 object's
//          slot 9 (camera-manager out getter); CGameCameraManager::ReleaseScriptedControl 0x00698950, ::ResetCamera 0x0069A990.
// Layout (retail, as the landed constructor 0x008F1CD0): Cards +0x128, Pack +0x134, ParticleEffects +0x148 (8-byte
//          CIntelligentPointer<CThing> elements), SelectedCard +0x308, HighlightedCard +0x30c; CCard::Thing +0x00; the
//          thing's dead flag is bit 0 of +0x91.
// Shape: the effect list and the card list are emptied with STLport's inline clear() = erase(begin(), end()): the
//        non-trivial element copy loop (`n = last - first` on the twice-loaded finish), then _Destroy through each
//        intelligent pointer's virtual destructor (`push 0 ; call [edx]`); the pack is drained by back()/pop_back();
//        the camera-manager out pointer is uninitialised (as ShoveHaPenny's LeaveGame 0x007CF460); the effect vector's
//        operator[] goes through an iterator OBJECT (`*(begin() + i)`), which gives the Kill call's
//        `mov eax,[first] ; lea ecx,[off+eax]` element address (landed AddNewCustomer 0x00798D90 idiom).
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
void __cdecl operator delete(void* p);                                                    // 0x00BFE9BC

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CBaseObject;
struct CThing {
    unsigned char _pad_0x00[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Kill(bool removeNow);                  // 0x004C9B80
};

struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual ~CBaseIntelligentPointer();         // slot 0 (the vector's _Destroy calls it virtually)
    CBaseObject* m_pItem;                       // +4
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    T* Get() const { return (T*)GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

template <class T> __forceinline T* CopyRange(T* first, T* last, T* result)   // STLport __copy (random access)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> __forceinline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

struct CEffectVector {                          // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    typedef CIntelligentPointer<CThing> T;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    struct iterator {                           // STLport iterator OBJECT (landed AddNewCustomer 0x00798D90 idiom)
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(_M_start); }
    T& operator[](unsigned long i) { return *(begin() + i); }
    __forceinline void clear()
    {
        T* i = CopyRange(_M_finish, _M_finish, _M_start);
        DestroyRange(i, _M_finish);
        _M_finish = i;
    }
};

struct CCard {
    CThing* Thing;                              // +0x00 CThingObject*
};
struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCard*& back() { return *(_M_finish - 1); }
    void pop_back() { --_M_finish; }
    void clear()
    {
        CCard** i = (CCard**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CGameCameraManager {
    void ReleaseScriptedControl();              // 0x00698950
    void ResetCamera();                         // 0x0069A990
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);     // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad[0x34];
    CCameraManagerSource* m_pCameraSource;      // +0x34
    CCameraManagerSource* GetCameraSource() { return m_pCameraSource; }
};

struct CTCTavernGame {
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    void LeaveGame();                           // 0x005EC650
    void KillThing(CThing* thing);              // 0x005E9A60
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
};
struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x38];
    CCardVector   Cards;                        // +0x128
    CCardVector   Pack;                         // +0x134
    unsigned char _pad_0x140[8];
    CEffectVector ParticleEffects;              // +0x148
    unsigned char _pad_0x154[0x1b4];
    CCard*        SelectedCard;                 // +0x308
    CCard*        HighlightedCard;              // +0x30c
    void LeaveGame();
};

void CTCTavernGameCardBase::LeaveGame()
{
    CTCTavernGame::LeaveGame();
    for (unsigned long i = 0; i < ParticleEffects.size(); ++i) {
        const CIntelligentPointer<CThing>& effect = ParticleEffects[i];
        if (effect.Get() && !effect.Get()->IsDead())
            ParticleEffects[i].Get()->Kill(true);
    }
    ParticleEffects.clear();
    Cards.clear();
    while (Pack.size() > 0) {
        CCard* card = Pack.back();
        Pack.pop_back();
        KillThing(card->Thing);
        delete card;
    }
    SelectedCard = 0;
    HighlightedCard = 0;
    CGameCameraManager* camera;
    GetWorld()->GetCameraSource()->GetCameraManager(&camera);
    camera->ReleaseScriptedControl();
    camera->ResetCamera();
}