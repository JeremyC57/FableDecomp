// NScript::CV_SingingStonesScript::Init() -- sets the play-list index and the normal/rude stone play lists, names the four stones "A".."D" and clears the door-man flags
// Leaf: ?Init@CV_SingingStonesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED0EC0, length 139 bytes (padded to 144), `ret`.
// Vtable 0x012F2B0C slot 3 (the landed ctor 0x00ED0E60, factory 0x00ED39A0 `push 9Ch`, registered at 0x00CD955C under
// "S_VSS" / "V_SingingStones"), so the catalog class is right.
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 (x4; literals "A" 0x0122DCE0, "B" 0x0122DCE4,
// "C" 0x012D8858, "D" 0x012CD3C0).
// Layout (retail = Ego_r.pdb - 0x14, then -0x18 after the 12-byte EffectList vector): CurrentPlayListIndex +0x48,
// PlayList[4] +0x4c, RudePlayList[4] +0x5c, YourPersonalPlayList[4] +0x6c, EffectList +0x7c, ListText[4] +0x88,
// DoorManIntroComplete +0x98, DoorManAttackedByHero +0x99, DoorManHasBribe +0x9a, DoorManComplete +0x9b.
// Naming wave 35: identity outer script vtable 012f2b0c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ed10a0 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CV_SingingStonesScript; PDB ?Init@CV_SingingStonesScript@NScript@@; refs=1; was CV_SingingStonesScript::Init (namespace spelling) (wave3/labels_alloc.tsv); PDB ?Init@CV_SingingStonesScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CV_SingingStonesScript@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const char* text);      // 0x0099EFE0
};
namespace NScript {
class CV_SingingStonesScript {
public:
    unsigned char _base[0x44];   // (-4: now the vptr of the virtual member)
    long CurrentPlayListIndex;                     // +0x48
    long PlayList[4];                              // +0x4c
    long RudePlayList[4];                          // +0x5c
    long YourPersonalPlayList[4];                  // +0x6c
    void* EffectList[3];                           // +0x7c (vector<CScriptThing>)
    CCharString ListText[4];                       // +0x88
    bool DoorManIntroComplete;                     // +0x98
    bool DoorManAttackedByHero;                    // +0x99
    bool DoorManHasBribe;                          // +0x9a
    bool DoorManComplete;                          // +0x9b
public:
    virtual void Init();                          // PDB ?Init@CV_SingingStonesScript@NScript@@UAEXXZ
};
}
using NScript::CV_SingingStonesScript;
void NScript::CV_SingingStonesScript::Init()
{
    CurrentPlayListIndex = 0;
    PlayList[0] = 3;
    PlayList[1] = 1;
    PlayList[2] = 0;
    PlayList[3] = 2;
    RudePlayList[0] = 2;
    RudePlayList[1] = 3;
    RudePlayList[2] = 1;
    RudePlayList[3] = 0;
    ListText[0] = "A";
    ListText[1] = "B";
    ListText[2] = "C";
    ListText[3] = "D";
    DoorManIntroComplete = false;
    DoorManAttackedByHero = false;
    DoorManHasBribe = false;
    DoorManComplete = false;
}