// AreAllThingsInVectorDead(const vector<CScriptThing>& things) -- true when every script thing in the vector is dead or unconscious
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?AreAllThingsInVectorDead@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBED00, 72 bytes, `__fastcall` global (vector in ecx), plain `ret`
//   (PDB ?AreAllThingsInVectorDead@@YI_NABV?$vector@VCScriptThing@@...@Z). No frame: everything lives in registers.
// Callees: CScriptThing (vtable 0x01238C8C) slot 75 IsAlive (+0x12c) and slot 61 IsUnconscious (+0xf4).
// Shape: a counted loop over the STLport vector (begin read once into edi, end re-read every pass because the virtual
//   calls may change it), `!IsAlive() || IsUnconscious()` bumps the count, then the count is compared with size()
//   (the 12-byte element divide `push 0Ch ; pop ecx ; cdq ; idiv` is the size TU's) and returned through `sete`.
class CScriptThing { public:                       // 12 bytes, vtable 0x01238C8C (vptr, then the counted PImp pointer pair)
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017();
    virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035();
    virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041();
    virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053();
    virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059();
    virtual void t060();
    virtual bool IsUnconscious() const;     // slot 61 (+0xf4, 0x004AAFA0)
    virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067();
    virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073();
    virtual void t074();
    virtual bool IsAlive() const;           // slot 75 (+0x12c, 0x004AB130)
    void* m_pImpData;                       // +0x04 CCountedPointer<CThing> PImp
    void* m_pImpInfo;                       // +0x08
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}

bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things)
{
    unsigned int dead = 0;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (!it->IsAlive() || it->IsUnconscious())
            ++dead;
    return dead == things.size();
}