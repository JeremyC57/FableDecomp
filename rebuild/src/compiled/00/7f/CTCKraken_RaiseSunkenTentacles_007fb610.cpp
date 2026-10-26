// CTCKraken::RaiseSunkenTentacles() -- for every tentacle thing in the Tentacles list take its CTCKrakenTentacle component (interface 0xE3) and schedule its rise: TickToRise = world frame + def rise delay * frames-per-second
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB610, 133 bytes. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, type 0xE3 = bit 3 of the dword at
// CThing+0x3C), GetWorldFrame 0x0049D870, __ftol2. Globals: frames-per-second long 0x01375550.
// Layout (retail, local model): CTCKraken PKrakenDef +0x0C (def float +0x78 = tentacle rise delay in seconds), Tentacles
// list<CIntelligentPointer<CThing> > +0x44 (PDB +0x4C: retail sits 8 bytes lower); CTCKrakenTentacle TickToRise +0x34 (PDB +0x34).
// Shape: the frames-per-second global and the def are named locals read before GetWorldFrame (retail order); the component out pointer is never reset, so a missing component keeps the previous one (retail ebp).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
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
extern long g_FramesPerSecond;                  // 0x01375550
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
        long fps = g_FramesPerSecond;
        CKrakenDef* def = PKrakenDef;
        tentacle->TickToRise = CWorld::GetFrame() + (long)(def->TentacleRiseDelay * fps);
    }
}