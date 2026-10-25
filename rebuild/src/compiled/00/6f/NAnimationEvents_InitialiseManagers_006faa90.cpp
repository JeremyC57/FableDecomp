// NAnimationEvents::InitialiseManagers(const CMeshDataBank& bank) -- create the game and sound animation-event managers over the mesh bank, each loading its event symbols from Data\Defs\game_animation_events.h / sound_animation_events.h
// Leaf: ?InitialiseManagers@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FAA90, 346 bytes, __fastcall (bank in ecx), plain `ret`.
// PDB: ?InitialiseManagers@NAnimationEvents@@YIXABVCMeshDataBank@@@Z.
// Callees: operator new 0x00BFEA1A (0x40 bytes), NResourceDirectoryNames::GetDefDir() 0x0041A080 (hidden result in ecx),
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30, CSymbolMap::CSymbolMap 0x009E5200, CSymbolMap::LoadFromFile
//   0x009E5C90, CCountedPointer<CManager>::Reset(CManager*) 0x006FD460, ~CWideString 0x0099B510.
// Data: the counted manager pointers 0x013BABB4 (game events) / 0x013BABBC (sound events) -- descriptive names, as in
//   the landed Save/LoadAnimationEvents; CManager vtable 0x01264974.
// Layout: retail NAnimationEvents::CManager (0x40 bytes): vptr, the mesh bank +4, EventSymbolMap +8 (CSymbolMap, 0x24),
//   AnimToExtraEventsMap +0x2c (CVectorMap: 12-byte vector, comparator +0x38, Dirty +0x39), DifferentToSavedCopy +0x3c,
//   UseEmbeddedEvents +0x3d (the landed EventExists / AddEventToAnim models).
// Shape: `GetXManager.Reset(new CManager(bank, GetDefDir() + L"...", flag))` twice with the constructor inlined; the two
//   string temporaries of each new-expression are destroyed under VC's conditional-temporary flag word (bits 1/2, 4/8).
// Lane note: wchar_t is not a built-in type in the lane's VC7.1 build, so the literal operand is `const unsigned short*`.
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A

class CWideString {                                       // 4 bytes
public:
    void* PStringData;                                    // +0x00
    ~CWideString();                                       // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

namespace NResourceDirectoryNames {
CWideString __fastcall GetDefDir();                       // 0x0041A080
}

class CMeshDataBank;

class CSymbolMap {
public:
    CSymbolMap();                                         // 0x009E5200
    bool LoadFromFile(const CWideString& fileName);       // 0x009E5C90
    unsigned char _body[0x24];
};

class CCPPointerInfo;
template<class T> class CCountedPointer {                 // Ego_r.pdb: 8 bytes
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    void Reset(T* data);                                  // 0x006FD460
};

namespace NAnimationEvents {
struct SExtraEventEntry;
struct CExtraEventMap {                                   // CVectorMap<unsigned long, CSmallVector<CEvent,32> > (16 bytes)
    SExtraEventEntry* First;                              // +0x00
    SExtraEventEntry* Last;                               // +0x04
    SExtraEventEntry* EndStorage;                         // +0x08
    unsigned char Compare;                                // +0x0c (empty comparator)
    bool Dirty;                                           // +0x0d
    CExtraEventMap() : First(0), Last(0), EndStorage(0), Dirty(false) {}
};

class CManager {
public:
    CManager(const CMeshDataBank& bank, const CWideString& fileName, bool useEmbeddedEvents)
        : MeshBank(&bank), DifferentToSavedCopy(false), UseEmbeddedEvents(useEmbeddedEvents)
    {
        EventSymbolMap.LoadFromFile(fileName);
    }
    virtual ~CManager();                                  // vtable 0x01264974
    const CMeshDataBank* MeshBank;                        // +0x04
    CSymbolMap EventSymbolMap;                            // +0x08
    CExtraEventMap AnimToExtraEventsMap;                  // +0x2c
    bool DifferentToSavedCopy;                            // +0x3c
    bool UseEmbeddedEvents;                               // +0x3d
};

extern CCountedPointer<CManager> GameEventManager;        // 0x013BABB4 (descriptive name)
extern CCountedPointer<CManager> SoundEventManager;       // 0x013BABBC (descriptive name)

void __fastcall InitialiseManagers(const CMeshDataBank& bank)
{
    GameEventManager.Reset(new CManager(bank, NResourceDirectoryNames::GetDefDir() + L"game_animation_events.h", true));
    SoundEventManager.Reset(new CManager(bank, NResourceDirectoryNames::GetDefDir() + L"sound_animation_events.h", false));
}
}