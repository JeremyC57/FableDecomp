// CTCActivationTrigger::OnInitialActivate() -- when a receptor UID is set, look the receptor thing up by UID, take its activation-receptor component (interface 0x65) when present, and register this trigger with the receptor
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077B1C0, length 97 bytes (+15 int3 padding). Vtable slot 6 of CTCActivationTrigger
// (0x0123BBAC; catalog "vfunc_6"; slot 6 is the CTCBase OnInitialActivate slot, as CTCFlammable 0x00780D20).
// Callees: the CTCBase global accessor @ 0x00686AE0 (`mov eax,[0x013B8A1C] ; ret`; catalog "GFGetPreMainMemoryUsed"), the
// UID lookup @ 0x0051ED80 (thiscall, 64-bit UID by value; catalog "CStopWatch::GetTicks"), CVectorMap<ETCInterfaceType,CTCBase*>
// ::LowerBound @ 0x0040F020 (inlined GetInterface), the receptor's trigger registration @ 0x007F7810 (catalog
// "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera").
// Shape: `ReceptorUID > 0` on the unsigned 64-bit UID (`test hi ; ja ; test lo ; jbe`); the UID pushes precede the accessor call
// (arguments before the object expression); GetInterface stores straight into PTCReceptor, which is re-read for the call.
// Layout: PDB CTCActivationTrigger (0x20): PTCReceptor +0xC, ReceptorUID +0x10, Activated +0x18; retail CThing component bitset
// +0x20 (type 0x65 = bit 5 of the dword at +0x2C) and interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
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
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingManager {
    CThing* GetThingByUID(unsigned __int64 uid);    // 0x0051ED80
};
class CTCActivationTrigger;
class CTCActivationReceptorBase {
public:
    void RegisterTrigger(CTCActivationTrigger* trigger);   // 0x007F7810
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CThingManager* GetThingManager();           // 0x00686AE0
};
class CTCActivationTrigger : public CTCBase {
public:
    CTCActivationReceptorBase* PTCReceptor;     // +0x0c
    unsigned __int64 ReceptorUID;               // +0x10
    bool Activated;                             // +0x18
    virtual void OnInitialActivate();
};

void CTCActivationTrigger::OnInitialActivate()
{
    if (ReceptorUID > 0) {
        CThing* receptor = GetThingManager()->GetThingByUID(ReceptorUID);
        receptor->GetInterface(&PTCReceptor, 0x65);
        PTCReceptor->RegisterTrigger(this);
    }
}