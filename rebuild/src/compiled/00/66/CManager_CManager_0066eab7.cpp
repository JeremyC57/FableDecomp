// NHeroInformationScreens::CManager::CManager(CWorld* world) -- hero information screen manager: 11 empty screen slots, reset scroll bar
// Leaf: ??0CManager@NHeroInformationScreens
// Retail: Fable.exe @ 0x0066EAB7, 90 bytes, __thiscall `ret 4`, frame-pointer size TU (`push ebp ; mov ebp,esp`).
// Screens is built with STLport's vector(size_type n, const T& val, const allocator&) (0x0066ECBE) for 11 null
// entries: the null value temporary is homed in the dead `world` slot ([ebp+8], after World is stored) and the
// allocator temporary in its high byte ([ebp+0Bh]). InventoryScreens / ScrollBarIndices are default-constructed
// inline (zero stores), then CurrentScreen, LastScreen, HighlightedScreen = 0 and OpenFlag = false, the
// ScreenEffectFade member ctor (0x00436560; manifest label CTCHeroStats::SoberUp is propagated) and finally
// InitialiseScrollBarIndices(5) (0x0066EA85, landed). Caller 0x0043BA7C.
// Layout: retail NHeroInformationScreens::CManager with 12-byte STLport vectors, as the landed 0x0066EA85 / 0x0066E941
// (Ego_r.pdb names; its 16-byte vectors put every member after Screens 4..12 bytes higher). engine/CManager.h is the
// unrelated NUISystem manager.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWorld;
namespace NHeroInformationScreens {
class CBase;
struct CAllocator { CAllocator() { } };
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit CVector(unsigned int n, const T& value = T(), const CAllocator& a = CAllocator());   // 0x0066ECBE for const CBase*
};
// Same-TU stand-in for vector<const CBase*>(n, const T&, const allocator&) 0x0066ECBE (body is not the retail one): the
// fill value is only read, so its address no longer escapes and VC7.1 homes the null temporary in the dead `world` slot.
void FillPointers_0066ECBE(void* vector, unsigned int n, const void* value);
#pragma auto_inline(off)
template <class T> __declspec(noinline) CVector<T>::CVector(unsigned int n, const T& value, const CAllocator& a)
{
    FillPointers_0066ECBE(this, n, value);
}
#pragma auto_inline(on)
class CFadeInFadeOutBase {
public:
    CFadeInFadeOutBase();                                // 0x00436560
    long _data[10];
};
class CManager {
public:
    CManager(CWorld* world);
    void InitialiseScrollBarIndices(long count);         // 0x0066EA85
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    long                  CurrentScreen;                 // +0x28
    long                  LastScreen;                    // +0x2c
    long                  HighlightedScreen;             // +0x30
    bool                  OpenFlag;                      // +0x34
    CFadeInFadeOutBase    ScreenEffectFade;              // +0x38
};
enum { NUM_HERO_INFORMATION_SCREENS = 11 };
CManager::CManager(CWorld* world)
    : World(world),
      Screens(NUM_HERO_INFORMATION_SCREENS),
      CurrentScreen(0),
      LastScreen(0),
      HighlightedScreen(0),
      OpenFlag(false)
{
    InitialiseScrollBarIndices(5);
}
}
