// CConsole::AddInput(CConsoleInputBase*) -- register input under a lowercase copy of its name, shared through a counted pointer.
// Retail 0x009EC5E0, 296 bytes. CCharString copy0099EC30, ToLower0099ED10, destructor0099EAE0;
// CConsole input map insert009EFB80, typed DeleteData009EDC50. Native RegisteredConsoleInputs+40 vs donor+48.
// Field-free native API adapter derives the actual shared string class: storage and destructor stay in CCharString.
#include "fable_string.h"
struct CRetailString : CCharString {
    CRetailString(); // 0x0099E4B0 native default
    CRetailString(const CRetailString&); // 0x0099EC30 native copy
    CRetailString ToLower() const; // 0x0099ED10
};
struct CConsoleInputBase {
    virtual ~CConsoleInputBase(); // real slot0 deleting destructor consumed by DeleteData
    CRetailString Name; // +4 native and donor layout
};
void FABLE_FASTCALL DeleteInputData(CConsoleInputBase*); // 0x009EDC50
struct SInputCountBlock {
    long refs;
    void (FABLE_FASTCALL* destroy)(CConsoleInputBase*);
    CConsoleInputBase* object;
    SInputCountBlock(CConsoleInputBase* p) : refs(1), destroy(DeleteInputData), object(p) { }
};
struct CInputPointer {
    CConsoleInputBase* object;
    SInputCountBlock* count;
    explicit CInputPointer(CConsoleInputBase* p) : object(p), count(p ? new SInputCountBlock(p) : 0) { }
    CInputPointer(const CInputPointer& other) : object(other.object), count(other.count) { if(count) ++count->refs; }
    ~CInputPointer() {
        if(count) { --count->refs; if(!count->refs) { count->destroy(count->object); delete count; } }
        object=0;count=0;
    }
};
struct CInputPair {
    CRetailString name;
    CInputPointer input;
    CInputPair(const CRetailString& n,const CInputPointer& p):name(n),input(p) { }
};
struct CInputValue {
    const CRetailString name;
    CInputPointer input;
    CInputValue(const CInputPair& p):name(p.name),input(p.input) { }
};
struct CInputInsertResult { void* iterator; bool inserted; CInputInsertResult(); };
struct CInputMap {
    CInputInsertResult Insert(const CInputValue&); // 0x009EFB80
};
struct CConsole {
    unsigned char _nativeBeforeRegisteredInputs[0x40];
    CInputMap RegisteredConsoleInputs;
    void AddInput(CConsoleInputBase*);
};
void CConsole::AddInput(CConsoleInputBase* input)
{
    CInputPointer pointer(input);
    RegisteredConsoleInputs.Insert(CInputPair(input->Name.ToLower(),pointer));
}