// CTCAssassinRush::SetFinalAction(CCountedPointer<CCreatureActionBase> pool) -- adopt the counted pointer into FinalAction (the parameter copy is released on return)
// Leaf: ?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ceae0, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z (bound in reloc_bindings with the stand-in parameter UCCountedAction); caller CCreatureAction_CastAssassinRush::Initialise 0x008CD470 (landed).
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb FinalAction (PDB +148) at +0x90.
// Header debt: engine/CTCAssassinRush.h is a flat layout without methods; modelled locally.
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer& other);
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                 // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CCreatureActionBase;
class CTCAssassinRush {
public:
    unsigned char _pad00[0x90];
    CCountedPointer<CCreatureActionBase> FinalAction;                          // +0x90
    void SetFinalAction(CCountedPointer<CCreatureActionBase> pool);
};
void CTCAssassinRush::SetFinalAction(CCountedPointer<CCreatureActionBase> pool)
{
    FinalAction = pool;
}