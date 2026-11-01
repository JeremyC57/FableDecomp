// CWorld::IsDisplayingMiniMapOnRegionLoad @ 0x0049e9f0
// Naming wave 18: the global stand-in g_pGlobalGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Loads a global pointer (ds:0x13b8790), reads a byte flag at +0xf6 on that
// global object; if the flag is set, returns false. Otherwise returns the byte
// field at this+0x12f. this = ecx (__fastcall).

class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public: char pad[0xf6]; bool suppressMiniMap; };

class CWorld { public:
    char pad[0x12f];
    bool bDisplayMiniMapOnRegionLoad;  // +0x12f
    bool IsDisplayingMiniMapOnRegionLoad();
};

bool CWorld::IsDisplayingMiniMapOnRegionLoad()
{
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().suppressMiniMap)
        return false;
    return bDisplayMiniMapOnRegionLoad;
}