// CTCGhostSword::IsThingGhostSword(const CThing* thing) -- true when the thing's definition is the "GHOSTLY_SWORD" def (looked up by name in the game definition manager; a missing def never matches)
// Leaf: ?IsThingGhostSword@CTCGhostSword
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C53F0, 76 bytes (+4 int3), static __fastcall (thing in ecx), bool in al.
// Identity: Ego_r.pdb name set ?IsThingGhostSword@CTCGhostSword@@SI_NPBVCThing@@@Z; callers 0x00811542, 0x00811672,
//   0x0083A798. The catalog label "IsThingGhostSword" is right but lacks the class.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("GHOSTLY_SWORD" @ 0x0125D2A4), CGameDefinitionManager::Get @
//   0x0044C6B0 (static __fastcall), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
//   ~CCharString @ 0x0099EAE0.
// Layout: retail CThing DefGlobalIndex (ushort) +0x8c (PDB +0x9c, the retail CThing is 0x10 shorter before it).
// Shape: the name is a named local in its own block (destroyed before the compare, its frame word the `push ecx`,
//   its address re-taken for the lookup argument; a temporary passes the ctor's eax instead); `index > 0 && DefGlobalIndex == index` as an if with `return true` / `return false` (`mov al,1`).
class CCharString {
public:
    void* PString;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
};

class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;              // +0x8c
};

class CTCGhostSword {
public:
    static bool __fastcall IsThingGhostSword(const CThing* thing);
};

bool __fastcall CTCGhostSword::IsThingGhostSword(const CThing* thing)
{
    long index;
    {
        CCharString name("GHOSTLY_SWORD");
        index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    if (index > 0 && thing->DefGlobalIndex == index)
        return true;
    return false;
}