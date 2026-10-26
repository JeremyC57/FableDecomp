// CTCOpinionSource::CleanupSource() -- drop the produced opinion set and remove every entry naming this source's thing from the world opinion manager's source list
// Retry 10b (getsw sweep, FIXES B-15): the opinion manager is read through an inline getter `GetOpinionManager()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771770, length 151 bytes (reached through the `jmp` thunks 0x00771810/0x00771820).
// Callees: operator delete @ 0x00BFE9BC (the counted pointer's info block after its __fastcall DeleteFunc), CTCBase::GetWorld
// @ 0x00686A70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the list element's virtual destructor (slot 0, flag 0), free
// @ 0x00BFEA14 (the list node).
// Layout (retail, local model): ParentThing +0x04; POpinionProduced +0x78 (CCountedPointer<COpinionSet>: Data +0x78, Info +0x7C;
// PDB +0x7C, the retail DefStack vector is 12 bytes); the world's opinion manager at CWorld+0x74 holds the source list
// (STLport list<CIntelligentPointer<CThing>>, sentinel pointer) at +0x71C.
// Shape: the counted-pointer release as the landed CTCLight one (Info then Data zeroed); the walk keeps the list reference in a named
// local, saves ParentThing in a named local before the GetPItem call (retail spills it to the frame), advances the next-node
// cursor before an inline erase (unlink, in-place virtual dtor, free) and then moves the node to it.
void __cdecl operator delete(void* block);          // 0x00BFE9BC
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
class CThing;
class CCPPointerInfo { public:                             // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();             // slot 0
    CThing* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct SSourceNode {
    SSourceNode* Next;
    SSourceNode* Prev;
    CBaseIntelligentPointer Source;                 // CIntelligentPointer<CThing>
};
struct CSourceList {                                // STLport list: sentinel pointer only
    SSourceNode* Node;
    void erase(SSourceNode* node)
    {
        SSourceNode* next = node->Next;
        SSourceNode* prev = node->Prev;
        prev->Next = next;
        next->Prev = prev;
        node->Source.~CBaseIntelligentPointer();
        free(node);
    }
};
struct COpinionManager {
    unsigned char _pad00[0x71c];
    CSourceList OpinionSources;                     // +0x71c
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionManager* POpinionManager;               // +0x74
    COpinionManager* GetOpinionManager() const { return POpinionManager; }
};
class COpinionSet;
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCOpinionSource : public CTCBase { public:
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x78 - 0x08];
    CCountedPointer<COpinionSet> POpinionProduced;  // +0x78
    void CleanupSource();
};

void CTCOpinionSource::CleanupSource()
{
    POpinionProduced.Release();
    CSourceList& sources = GetWorld().GetOpinionManager()->OpinionSources;
    SSourceNode* node = sources.Node->Next;
    SSourceNode* it = node;
    while (node != sources.Node) {
        CThing* owner = ParentThing;
        it = it->Next;
        if (node->Source.GetPItem() == owner)
            sources.erase(node);
        node = it;
    }
}