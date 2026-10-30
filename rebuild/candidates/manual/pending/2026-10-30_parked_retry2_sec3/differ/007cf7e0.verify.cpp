// CTCTavernGameShoveHaPenny::NewRound() -- coin-base NewRound, clear the board, find the obstacles, add the first coin at the board's centre line on the def's minimum coin position, then go to state 15
// Grade: RELOCATION_MATCH -- verification copy: engine/CDefPointer.h text inlined with the proposed ENullDef ctor (not for landing)
// Leaf: ?NewRound@CTCTavernGameShoveHaPenny@@
// Retail: Fable.exe @ 0x007CF7E0, 127 bytes.
// Callees: CTCTavernGameCoinBase::NewRound() 0x008EF180 (non-virtual base call; manifest "CXMVPlaybackInfo" ctor label is a
//          propagation); CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer*) 0x007D1260 on the owner's
//          def holder (CThing+0x70; manifest "<CAICreatureWillPowerIndicatorDef>" is a propagated template label);
//          CTCTavernGameCoinBase::ClearBoard() 0x008EF950, ::FindObstacles() 0x008EFB00, ::AddNewCoin(const C3DVector&)
//          0x008EFA00; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's slot 1 on release. Float pool
//          0x0122F59C = 0.5.
// Layout: retail CTCTavernGameCoinBase::BoardSize +0x130 (PDB +0x14c; retail BoardPos +0x10c, BoardRHSet +0x118);
//         CTavernGameShoveHaPennyDef::PositionCoinMinimum +0x1c8 (PDB offset holds).
// Shape: the sub-def is copied once out of the escaped out slot (after the two board calls) into a holder that releases it
//        (`dec [edi+4] ; jne`, no null test); the start position is a 3-float temporary.
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};


class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CTavernGameDef : public CSubDefClassBase { };          // RTTI .?AVCTavernGameDef@@ : CSubDefClassBase
class CTavernGameShoveHaPennyDef : public CTavernGameDef {    // RTTI .?AVCTavernGameShoveHaPennyDef@@ : CTavernGameDef
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernGameShoveHaPennyDef", -1); }   // inline
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad[0x1c8 - sizeof(CTavernGameDef)];
    float PositionCoinMinimum;                  // +0x1c8
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 (landed): its retail body in the TU (same-TU mod-ref, out of line)
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer<const CTavernGameShoveHaPennyDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameShoveHaPennyDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class CThingBaseDef : public CDefClassBase { };
class CThing {
public:
    unsigned char _pad_0x00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;  // +0x70
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }   // Ego_r ?PeekBaseDef@CThing@@QBEABVCThingBaseDef@@XZ (inline)
};
class CTCTavernGameCoinBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x128];
    float BoardSizeX;                           // +0x130 BoardSize.X
    void NewRound();                            // 0x008EF180
    void ClearBoard();                          // 0x008EF950
    void FindObstacles();                       // 0x008EFB00
    void AddNewCoin(const C3DVector& pos);      // 0x008EFA00
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
};
class CTCTavernGameShoveHaPenny : public CTCTavernGameCoinBase {
public:
    void NewRound();
};

void CTCTavernGameShoveHaPenny::NewRound()
{
    CTCTavernGameCoinBase::NewRound();
    CDefPointer<const CTavernGameShoveHaPennyDef> def(NULL_DEF);
    ParentThing->PeekBaseDef().GetSubDef(&def);
    ClearBoard();
    FindObstacles();
    AddNewCoin(C3DVector(0.5f * BoardSizeX, def->PositionCoinMinimum, 0.0f));
    SetState(15);
}
