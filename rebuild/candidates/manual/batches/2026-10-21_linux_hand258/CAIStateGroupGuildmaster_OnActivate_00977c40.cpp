// CAIStateGroup_Guildmaster::OnActivate() -- find the bodyguards, collect every "GuildmasterTeleportMarker" track thing within 55 units of the guildmaster as teleport markers, reset the will/teleport frames and make sure the creature is in mode 0x3b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977C40, length 436 bytes (+ int3 padding; `and esp,-8` frame for the 8-byte
// intelligent-pointer temporary).
// Callees: CAIStateGroup_Guildmaster::FindAndStoreBodyguards @ 0x00977B10, CCharString(const char*, long) @ 0x0099EBF0 / copy
// ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0 ("GuildmasterTeleportMarker" @ 0x01298840), CThing::GetPosition @ 0x004C73D0,
// __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, the rough-area collector @ 0x00978250 (three
// arguments: box, filter, output vector; the manifest "GetPBestThingInRoughArea<HasTrackName, Nearest, CThingTrackNode>"
// is a BSim guess with five), CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
// dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), the member vector's _M_insert_overflow @ 0x004AC940,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 = bit 17 of +0x24), CTCCreatureModeManager::HasMode
// @ 0x0073A8A0 / AddMode @ 0x0073AE10, the CRT free @ 0x00BFEA14 (the local vector's storage).
// Shape: the name and a NAMED filter copy are built before the position (one GetPosition, x then y truncated); the box is a
// temporary; a same-TU noinline stand-in for the collector (retail instantiates it in this TU) lets VC keep the local vector's
// start in esi across the loop (reloaded at the latch and reused by the final free); size() cached before an unsigned loop.
// Layout: Guildmaster from Ego_r.pdb (base 0x14): TeleportMarkers +0x28, LastWillFrame +0x34 and LastTeleportFrame +0x38 (PDB
// +0x38 / +0x3c: the vector is 12 bytes in STLport).
struct C3DVector {
    float X, Y, Z;
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
extern "C" void __cdecl free(void* block);    // 0x00BFEA14
struct CThingPtrVector {                        // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CThing*& operator[](unsigned int n) { return _M_start[n]; }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, CThingPtrVector& out) const;   // 0x00978250
};
long CollectSink_977c40(const void* self, long seen, CThing*** slots);   // stand-in: the area walk the collector does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for the rough-area collector @ 0x00978250 (a template instantiated in retail's TU): it reads its const
// reference arguments and fills the output vector, and lets no argument escape.
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, CThingPtrVector& out) const
{
    out._M_finish = out._M_start + CollectSink_977c40(this, area.MinX + *(const long*)&filter, 0);
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool HasMode(unsigned long mode);           // 0x0073A8A0
    bool AddMode(unsigned long mode);           // 0x0073AE10
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    long AttackNum;                             // +0x14
    CBaseIntelligentPointer PEnemy;             // +0x18 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PLastTeleportMarker;   // +0x20 CIntelligentPointer<CThing>
    CRetailVector<CIntelligentPointer> TeleportMarkers;   // +0x28
    long LastWillFrame;                         // +0x34
    long LastTeleportFrame;                     // +0x38
    void FindAndStoreBodyguards();              // 0x00977B10
    virtual void OnActivate();
};

void CAIStateGroup_Guildmaster::OnActivate()
{
    FindAndStoreBodyguards();
    CThingPtrVector markers;
    {
        CCharString name("GuildmasterTeleportMarker");
        CThingFilter_HasTrackName filter(name);
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 55, y - 55, x + 55, y + 55), filter, markers);
    }
    unsigned int count = markers.size();
    for (unsigned int i = 0; i < count; ++i)
        TeleportMarkers.push_back(CIntelligentPointer(markers[i]));
    LastWillFrame = 0;
    LastTeleportFrame = 0;
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    if (!modes->HasMode(0x3b))
        modes->AddMode(0x3b);
}
