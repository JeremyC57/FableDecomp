// CTCKraken::RaiseSunkenTentacles() -- for every tentacle thing in the Tentacles list take its CTCKrakenTentacle component (interface 0xE3) and schedule its rise: TickToRise = world frame + def rise delay * frames-per-second
// Grade: DIFFER(137v133) -- the frames-per-second global is copied into its own frame word (sub esp,8); retail reuses the GetInterface key slot for it and for the spilled world frame (push ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB610, 133 bytes. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, type 0xE3 = bit 3 of the dword at
// CThing+0x3C), GetWorldFrame 0x0049D870, __ftol2. Globals: frames-per-second long 0x01375550.
// Layout (retail, local model): CTCKraken PKrakenDef +0x0C (def float +0x78 = tentacle rise delay in seconds), Tentacles
// list<CIntelligentPointer<CThing>> +0x44 (PDB +0x4C: retail sits 8 bytes lower); CTCKrakenTentacle TickToRise +0x34 (PDB +0x34).
// Shape: the frames-per-second global and the def are named locals read before GetWorldFrame (retail order); the component out pointer is never reset, so a missing component keeps the previous one (retail ebp).
class CTCBase;
struct CInterfaceMap {
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CListNode {
    CListNode* Next;
    CListNode* Prev;
    T Data;
};
template<class T> struct CList {               // STLport list: header node pointer
    CListNode<T>* Node;
};
extern long g_FramesPerSecond;                  // 0x01375550
long GetWorldFrame();                           // 0x0049D870
struct CKrakenDef {
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
    CList<CBaseIntelligentPointer> Tentacles;   // +0x44
    void RaiseSunkenTentacles();
};

void CTCKraken::RaiseSunkenTentacles()
{
    CTCKrakenTentacle* tentacle;
    for (CListNode<CBaseIntelligentPointer>* it = Tentacles.Node->Next; it != Tentacles.Node; it = it->Next) {
        it->Data.GetPItem()->GetInterface(&tentacle, 0xe3);
        long fps = g_FramesPerSecond;
        CKrakenDef* def = PKrakenDef;
        tentacle->TickToRise = GetWorldFrame() + (long)(def->TentacleRiseDelay * fps);
    }
}
