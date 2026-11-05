// YesCloseInventory() -- the "yes" answer of the experience screen's close prompt: if the main player's character is alive, close its experience inventory with undo
// Naming wave 35c (queue land0411j 0x00487B70): calls 0x00487B70 as CPlayer::GetPPlayerCharacterThing() const (Ego_r.pdb ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ; was GetPlayerCharacter).
// Leaf: ?YesCloseInventory@@YIXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FBED, 112 bytes, `void __fastcall YesCloseInventory()` (FableWin.pdb
//   `?YesCloseInventory@@YIXXZ`, a free yes/no callback). Frame-pointer size TU, as the neighbouring landed
//   CTCInventoryExperience::OpenInventory (0x0056FAD7): optimize("y", off) + ("s", on).
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960 (the main game component 0x013B86A0 +0x1C),
//   CPlayer::GetPPlayerCharacterThing() const @ 0x00487B70, IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (__fastcall, by reference),
//   std::bitset::test @ 0x00410DE0 + CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for
//   interface 0x16, the experience inventory), and that component's virtual slot 89 (+0x164) given true (the
//   landed 0x0057770A body: slot 89, `bool undo` -- give back the undoable purchases, then close).
// Shape: the component pointer is an uninitialised out local; VC7.1 homes it, the hero and the interface key in
//   ONE frame word ([ebp-4]): the miss path calls through whatever that word holds (the hero), as retail does.
//   The hero stays in esi across IS_THING_ALIVE only with that template's body in the TU (noinline stand-in).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CPlayer {
public:
    CThing* GetPPlayerCharacterThing() const;      // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};

// 0x0040DCE0 (COMDAT-folded IS_THING_ALIVE<T*> instantiations): the header template, visible here and out of line
// as in retail, so VC7.1 knows the hero is only read through the reference.
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && thing->IsDead() == 0;
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
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer()->GetPPlayerCharacterThing();
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory;
        hero->GetTC(TCI_INVENTORY_EXPERIENCE, &inventory);
        inventory->CloseInventoryInternal(true);
    }
}