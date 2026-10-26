// CQ_HangingTreeGoodScript::RemoveDregs() -- empty CurrentEnemyVector from the back: every enemy still alive is faded out and killed (0.5 s), then the element is popped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6FD00, length 101 bytes (padded to 112).
// Callees: CScriptThing::IsAlive (virtual slot 75, +0x12c; 0x004AB130), FadeOutAndKillEntity @ 0x00CBE716 (`__fastcall`
// (thing in ecx, the first bool in dl, float and second bool on the stack)), CScriptThing's destructor through vtable
// slot 0 with 0 (the STLport pop_back `_Destroy`).
// Layout (retail = Ego_r PDB offsets -0x14; the retail STLport vector is 12 bytes): CurrentEnemyVector +0xa0 (_M_start)
// / +0xa4 (_M_finish).
// Shape: `while (!empty())` with the inline back() (`*(end() - 1)`, re-read for every use) and pop_back
// (`--_M_finish; _M_finish->~CScriptThing();`, the explicit virtual destructor call passes 0).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    void* PImp_Data;                               // +0x04
    void* PImp_Info;                               // +0x08
};
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool removeOnFade);   // 0x00CBE716
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    T& back() { return *(_M_finish - 1); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    unsigned char              _pad_0x00[0xa0];
    CRetailVector<CScriptThing> CurrentEnemyVector;    // +0xa0
    void RemoveDregs();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;
void NScript::CQ_HangingTreeGoodScript::RemoveDregs()
{
    while (!CurrentEnemyVector.empty()) {
        if (CurrentEnemyVector.back().IsAlive())
            FadeOutAndKillEntity(CurrentEnemyVector.back(), 0.5f, true, true);
        CurrentEnemyVector.pop_back();
    }
}