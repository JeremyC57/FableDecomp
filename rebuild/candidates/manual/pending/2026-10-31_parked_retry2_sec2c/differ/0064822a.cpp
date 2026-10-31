// CDrawCombatMultiplier::Draw() -- HEAD DRAFT (first block of 1526 bytes): clear every primitive handle of the HUD's handle
// Grade: DIFFER(121v1526) 532 diff lines (skeleton 3v1526/550): head draft (handle for_each Clear, hero alive, TC 0x68 query); callee 0x006D6570 name unresolved (bindings: CTCHeroStats::GetComboMultiplier / catalog CTCHeroExperience::GetExperiencePickUpMultiplier) (retry wave 2 sec2c)
// vector (for_each + mem_fun_ref, 0x0043F360 / 0x0041BE70), then with the hero alive and owning TC 0x68 query it
// (0x006D6570); the x87 multiplier meter layout is not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064822A, 1526 bytes; /Os ebp frame 0x26c.
#pragma optimize("s", on)
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key) const; };                  // 0x004365B0
class CTCBitset { public: unsigned long W[9]; bool test(long pos) const; };   // 0x00410DE0
class CThing { public: unsigned char _pad00[0x20]; CTCBitset UsingComponents; CVectorMapTC Components; };
class CThingPlayerCreature : public CThing { };
bool __fastcall IS_THING_ALIVE(const CThingPlayerCreature* const& thing);   // 0x0043DBF0
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };              // 0x00449970
class CEnginePrimitiveHandle { public: void* Data; void Clear(); };     // 0x0041BE70
struct CMemFunRefClear { void (CEnginePrimitiveHandle::*Fn)(); };
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear fn);   // 0x0043F360
class CTCCombatMultiplier { public: long GetMultiplier(); };            // 0x006D6570
class CDrawCombatMultiplier {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                     // +0x08
    unsigned char _pad0c[0x1c - 0x0c];
    CEnginePrimitiveHandle* HandlesFirst;                               // +0x1c
    CEnginePrimitiveHandle* HandlesLast;                                // +0x20
    void Draw();
};
void CDrawCombatMultiplier::Draw()
{
    CMemFunRefClear clear = { &CEnginePrimitiveHandle::Clear };
    for_each(HandlesFirst, HandlesLast, clear);
    const CThingPlayerCreature* hero = PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    if (!hero->UsingComponents.test(0x68))
        return;
    long key = 0x68;
    const CVectorMapTC::value_type* it = hero->Components.LowerBound(key);
    if (it == hero->Components._M_finish || key < it->first)
        it = hero->Components._M_finish;
    CTCCombatMultiplier* multiplier = (CTCCombatMultiplier*)it->second;
    multiplier->GetMultiplier();
}
