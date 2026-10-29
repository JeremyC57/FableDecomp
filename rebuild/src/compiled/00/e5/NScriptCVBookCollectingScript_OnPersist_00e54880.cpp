// NScript::CV_BookCollectingScript::OnPersist(CPersistContext& context) -- save/load the book-collecting quest: intro and reward flags, the gossip state, the information flag, the donation counters and the last requested book, then the donated / owned book bit vectors, each under its member name
// Leaf: ?OnPersist@CV_BookCollectingScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54880, 269 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012E5EFC,
//   which the landed NScript::CV_BookCollectingScript ctor 0x00E54310 stores (slot 1 the landed RegisterMain 0x00E543B0);
//   Ego_r.pdb ?OnPersist@CV_BookCollectingScript@NScript@@UAEXAAVCPersistContext@@@Z (0x00EAB7D0). Every persisted name
//   string is the Ego_r.pdb member at that retail offset. Catalog label sub_00e54880.
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / Transfer<long> 0x00410BE0 (Ego_r ??$Transfer@_N@... / ??$Transfer@J@...),
//   TransferVector<bool> 0x00CDCF80 (Ego_r ??$TransferVector@_N@CPersistContext@@QAEXPBDAAV?$vector@_NV?$allocator@_N@std@@@std@@@Z).
// Strings (paired by hand): "DoneIntro" 0x012E3CF4, "HatRewarded" 0x012E5FE0, "KeyRewarded" 0x012E5FD4, "GossipState"
//   0x012E5FC8, "HasInformation" 0x01272884, "BooksDonated" 0x012E5FB8, "GoodBooksDonated" 0x012E5FA4,
//   "LastBookRequested" 0x012E5F90, "BookDonated" 0x012E5F84, "BookOwned" 0x012E5F78.
// Layout: the landed ctor 0x00E54310 (retail = Ego_r - 0x14 up to BookReactions, STLport vectors 12 bytes): DoneIntro +0x78,
//   GossipState +0x7c, HasInformation +0x80, BooksDonated +0x84 .. LastBookRequested +0x8c, BookDonated +0xa0, BookOwned
//   +0xac (retail size 0xB8: vector<bool> is three words here, as the FireHeart rows found).
// Layout: engine/CBaseObject.h is flattened (its vptr is a data member), so CBaseObject / CScriptGameResourceObjectScriptedThingBase
//   are modelled locally as polymorphic classes (only their 0x10 size matters here; nothing of them is called).
// Shape: the landed CV_ChapelOfEvilScript::OnPersist 0x00E690F0 model: one Transfer per member in retail's order (ebx = 0
//   feeds both the bool and the long defaults); the two vectors go through TransferVector without a default.
class CCharString { public: void* PData; };
class CConversation;
class CGameScriptInterfaceBase;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 <bool>, 0x00410BE0 <long>
    template <class T> void TransferVector(const char* name, std::vector<T>& value);         // 0x00CDCF80 <bool>
};
class CBaseObject {                                     // CBaseIntelligentPointer root
public:
    virtual ~CBaseObject();                             // 0x0099A430
    void* m_pIntelligentPointer;                        // +0x04
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject {   // vtable 0x0127094C, 0x10 bytes
public:
    void* PImp_Data;                                    // +0x08
    void* PImp_Info;                                    // +0x0c
};
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool ReadingBook;                                   // +0x48
    std::vector<CConversation> BookReactions;           // +0x4c
    CScriptGameResourceObjectScriptedThingBase seh_Boy; // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl;// +0x68
    bool DoneIntro;                                     // +0x78
    bool HatRewarded;                                   // +0x79
    bool KeyRewarded;                                   // +0x7a
    long GossipState;                                   // +0x7c
    bool HasInformation;                                // +0x80
    long BooksDonated;                                  // +0x84
    long GoodBooksDonated;                              // +0x88
    long LastBookRequested;                             // +0x8c
    long BooksInGame;                                   // +0x90
    std::vector<CCharString> BookDef;                   // +0x94
    std::vector<bool> BookDonated;                      // +0xa0
    std::vector<bool> BookOwned;                        // +0xac
    virtual void OnPersist(CPersistContext& context);
};

void CV_BookCollectingScript::OnPersist(CPersistContext& context)
{
    context.Transfer("DoneIntro", DoneIntro, false);
    context.Transfer("HatRewarded", HatRewarded, false);
    context.Transfer("KeyRewarded", KeyRewarded, false);
    context.Transfer("GossipState", GossipState, 0L);
    context.Transfer("HasInformation", HasInformation, false);
    context.Transfer("BooksDonated", BooksDonated, 0L);
    context.Transfer("GoodBooksDonated", GoodBooksDonated, 0L);
    context.Transfer("LastBookRequested", LastBookRequested, 0L);
    context.TransferVector("BookDonated", BookDonated);
    context.TransferVector("BookOwned", BookOwned);
}
}