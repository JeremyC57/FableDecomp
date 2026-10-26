// YesCloseInventory() -- the "yes" answer of the experience screen's close prompt: if the main player's character is alive, close its experience inventory with undo
// Leaf: ?YesCloseInventory@@YIXXZ
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FBED, 112 bytes, `void __fastcall YesCloseInventory()` (FableWin.pdb
//   `?YesCloseInventory@@YIXXZ`, a free yes/no callback). Frame-pointer size TU, as the neighbouring landed
//   CTCInventoryExperience::OpenInventory (0x0056FAD7): optimize("y", off) + ("s", on).
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960 (the main game component 0x013B86A0 +0x1C),
//   CPlayer::GetPlayerCharacter @ 0x00487B70, IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (__fastcall, by reference),
//   std::bitset::test @ 0x00410DE0 + CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for
//   interface 0x16, the experience inventory), and that component's virtual slot 89 (+0x164) given true (the
//   landed 0x0057770A body: slot 89, `bool undo` -- give back the undoable purchases, then close).
// Shape: the component pointer is an uninitialised out local; VC7.1 homes it, the hero and the interface key in
//   ONE frame word ([ebp-4]): the miss path calls through whatever that word holds (the hero), as retail does.
//   The hero stays in esi across IS_THING_ALIVE only with that template's body in the TU (noinline stand-in).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char DeadFlags;                         // +0x91 (bit 0: dead; retail)
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    CThing* GetPlayerCharacter();                    // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

// 0x0040DCE0 (COMDAT-folded IS_THING_ALIVE<T*> instantiations): the header template, visible here and out of line
// as in retail, so VC7.1 knows the hero is only read through the reference.
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && (thing->DeadFlags & 1) == 0;
}

class CTCInventoryExperience { public:                      // interface type 0x16
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void CloseInventoryInternal(bool undo);  // slot 89 (+0x164)
};

void __fastcall YesCloseInventory()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory;
        hero->GetInterface(&inventory, 0x16);
        inventory->CloseInventoryInternal(true);
    }
}