// CTCFireheartMinigame::PlaySoundW(const CCharString& name) -- play `name` on the parent thing's sound player; the invalid handle when it has none
// Leaf: ?PlaySoundW@CTCFireheartMinigame@@QAEKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7880, 88 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?PlaySoundW@CTCFireheartMinigame@@QAEKABVCCharString@@@Z (public; PlaySoundW is windows.h's
//   PlaySound); its caller is CTCFireheartMinigame::DelayedQuit 0x007E7AC0. The manifest's "CTCBetting::PlaySoundW" is propagated across byte-identical bodies.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetInterface, interface 16),
//   CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) 0x0066BD00; g_InvalidSoundHandle 0x0129D25C.
// Shape (family M163, representative 0x005EA950): `CTCSoundPlayer* sound; if (ParentThing->GetInterface(&sound, 16)) return
//   sound->PlayAttachedCriteria(name, true, false, false, -1); return g_InvalidSoundHandle;` -- the landed
//   CTCWillLightning::StartLoopingSound 0x007A0400 GetInterface; the interface key in the `push ecx` word. Speed TU.
class CCharString {
public:
    void* PStringData;
};
extern unsigned long g_InvalidSoundHandle;                // 0x0129D25C
class CTCBase;
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                 // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                       // +0x20
    CInterfaceMap m_interfaces;                           // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCFireheartMinigame {
public:
    void*   __vftable;                                    // +0x00 (CTCBase)
    CThing* ParentThing;                                  // +0x04
public:
    unsigned long PlaySoundW(const CCharString& name);
};
unsigned long CTCFireheartMinigame::PlaySoundW(const CCharString& name)
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 16))
        return sound->PlayAttachedCriteria(name, true, false, false, -1);
    return g_InvalidSoundHandle;
}