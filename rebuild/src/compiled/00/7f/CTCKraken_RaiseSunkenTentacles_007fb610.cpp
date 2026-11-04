// CTCKraken::RaiseSunkenTentacles() -- for every tentacle thing in the Tentacles list take its CTCKrakenTentacle component (interface 0xE3) and schedule its rise: TickToRise = world frame + def rise delay * frames-per-second
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB610, 133 bytes. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, type 0xE3 = bit 3 of the dword at
// CThing+0x3C), GetWorldFrame 0x0049D870, __ftol2. Globals: frames-per-second long 0x01375550.
// Layout (retail, local model): CTCKraken PKrakenDef +0x0C (def float +0x78 = tentacle rise delay in seconds), Tentacles
// list<CIntelligentPointer<CThing> > +0x44 (PDB +0x4C: retail sits 8 bytes lower); CTCKrakenTentacle TickToRise +0x34 (PDB +0x34).
// Shape: the frames-per-second global and the def are named locals read before GetWorldFrame (retail order); the component out pointer is never reset, so a missing component keeps the previous one (retail ebp).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
template<class T> struct CListNode {
    CListNode* Next;
    CListNode* Prev;
    T Data;
};
template<class T> struct CList {               // STLport list: header node pointer
    CListNode<T>* Node;
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                           // 0x0049D870
class CKrakenDef { public:
    unsigned char _pad00[0x78];
    float TentacleRiseDelay;                    // +0x78
};
class CTCKrakenTentacle {
public:
    unsigned char _pad00[0x34];
    long TickToRise;                            // +0x34
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
class CTCKraken : public CTCBase {
public:
    CKrakenDef* PKrakenDef;                     // +0x0c
    unsigned char _pad10[0x44 - 0x10];
    CList<CIntelligentPointer<CThing> > Tentacles;   // +0x44
    void RaiseSunkenTentacles();
};

void CTCKraken::RaiseSunkenTentacles()
{
    CTCKrakenTentacle* tentacle;
    for (CListNode<CIntelligentPointer<CThing> >* it = Tentacles.Node->Next; it != Tentacles.Node; it = it->Next) {
        it->Data.GetPItem()->GetInterface(&tentacle, 0xe3);
        long fps = CMainGameComponent::GetConstantFPS();
        CKrakenDef* def = PKrakenDef;
        tentacle->TickToRise = CWorld::GetFrame() + (long)(def->TentacleRiseDelay * fps);
    }
}