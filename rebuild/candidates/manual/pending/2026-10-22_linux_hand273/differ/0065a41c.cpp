// CTCVillage::GetFineAmountForCurrentCrimes() -- sum the fines of the hero's logged crimes in this village (unique crime types counted once when flagged 0x20), net of the village's crime credit; -1 fine aborts
// Grade: DIFFER(383v415) -- register allocation: retail keeps this in ebx, the village pointer in edi (spilled to [ebp-18h]) and fine in memory [ebp-14h]; ours puts this in edi (spilled) and fine in ebx, and merges the null-village exit into the release path (retail jumps straight to `return 0`, release without a null test)
// Leaf: GetFineAmountForCurrentCrimes
// Flags: /GS
// Callees: GetWorldMap-copy 0x686D30 (byte-identical to 0x686D20), CThing::GetPosition 0x4C73D0, CWorldMap vslot 16, 0x4FC190, 0x4FB490 (region record 0x58 bytes, village info +0x24),
//   GetMainPlayer 0x49D9D0, GetHeroThing 0x487DC0, bitset::test 0x410DE0, LowerBound 0x40F020, GetDeedFromHandle 0x6C58F0, deed-log GetDeedDef 0x6C5630, tree increment 0x4292C0.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
struct C3DVector { float X, Y, Z; };
struct CBitset {
    unsigned long Bits[9];
    bool test(unsigned int pos) const;                      // 0x00410DE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_tcBits;                                 // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) const { return m_tcBits.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CPlayer { CThing* GetHeroThing(); };                 // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };        // 0x0049D9D0
struct CMainGameComponent { unsigned char _pad00[0x24]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CVillageInfo {
    virtual void v00();
    virtual void DeleteThis();                              // slot 1
    long RefCount;                                          // +0x04
    unsigned char _pad08[0x9d - 0x08];
    bool HasLaw;                                            // +0x9d
};
struct CVillageInfoPointer {
    CVillageInfo* P;
    CVillageInfoPointer(CVillageInfo* p) : P(p) { if (P) ++P->RefCount; }
    ~CVillageInfoPointer() { if (P) { if (--P->RefCount == 0) P->DeleteThis(); } }
};
struct CRegionInfo {                                        // 0x58 bytes
    unsigned char _pad00[0x24];
    CVillageInfo* Village;                                  // +0x24
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapAtPosition(const C3DVector& pos);    // slot 16
    long GetRegionNumberMapIsIn(long map);                  // 0x004FC190
    CRegionInfo* GetRegionInfo(long region);                // 0x004FB490
};
struct CDeed { long Type; };
struct CDeedDef {
    virtual void v00();
    virtual void DeleteThis();                              // slot 1
    long RefCount;                                          // +0x04
    unsigned char _pad08[0x74 - 0x08];
    long FineAmount;                                        // +0x74
    unsigned char _pad78[4];
    unsigned char Flags;                                    // +0x7c
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDeedDefPointer {
    CDeedDef* Object;
    __forceinline ~CDeedDefPointer() { if (Object) { Object->Release(); Object = 0; } }
    CDeedDef* operator->() const { return Object; }
};
struct CTCHeroOpinionDeedLog {                              // the hero's TC 0x51
    CDeed* GetDeedFromHandle(long handle);                  // 0x006C58F0
    CDeedDefPointer GetDeedDef(long type);                  // 0x006C5630
};
struct CLongSetNode {
    long  Color;
    CLongSetNode* Parent;
    CLongSetNode* Left;
    CLongSetNode* Right;
    long  Value;                                            // +0x10
};
CLongSetNode* IncrementNode(CLongSetNode* node);            // 0x004292C0
struct CLongSet {
    CLongSetNode* Header;
    long  NodeCount;
    long  Pad;
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorldMap* GetWorldMap() const;                         // 0x00686D30
};
struct CTCVillage : CTCBase {
    unsigned char _pad08[0x64 - 0x08];
    CLongSet PlayersCrimes;                                 // +0x64
    unsigned char _pad70[0xa0 - 0x70];
    long CrimeCredit;                                       // +0xa0
    long GetFineAmountForCurrentCrimes();
};

long CTCVillage::GetFineAmountForCurrentCrimes()
{
    CVillageInfoPointer village = GetWorldMap()->GetRegionInfo(GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapAtPosition(ParentThing->GetPosition())))->Village;
    if (village.P && village.P->HasLaw) {
        long fine = -CrimeCredit;
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero) {
            CTCHeroOpinionDeedLog* deedLog;
            if (hero->HasTC(0x51))
                deedLog = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
            bool seen[10];
            memset(seen, 0, sizeof(seen));
            CLongSetNode* end = PlayersCrimes.Header;
            for (CLongSetNode* node = end->Left; node != end; node = IncrementNode(node)) {
                CDeed* deed = deedLog->GetDeedFromHandle(node->Value);
                if (deed) {
                    CDeedDefPointer def = deedLog->GetDeedDef(deed->Type);
                    if (def->FineAmount == -1) {
                        fine = -1;
                        break;
                    }
                    if (!(def->Flags & 0x20) || !seen[deed->Type])
                        fine += def->FineAmount;
                    seen[deed->Type] = true;
                }
            }
        }
        if (fine < 0) {
            CrimeCredit = -fine;
            fine = 0;
        }
        return fine;
    }
    return 0;
}
