// MoveAllThingsInVectorToPosition(const vector<CScriptThing>& things, const C3DVector& position) -- take every living script thing in the vector under scripted control and send it to the position
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?MoveAllThingsInVectorToPosition@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF679, 99 bytes, `__fastcall` global (vector in ecx, position in edx),
//   plain `ret` (PDB ?MoveAllThingsInVectorToPosition@@YIXABV?$vector@VCScriptThing@@...@@ABVC3DVector@@@Z). No callers
//   in the retail image.
// Label: the catalog name "KillAllThingsInVector" is propagated from 0x00CBED82; this body starts scripting each thing
//   and moves it, and it is the PDB's MoveAllThingsInVectorToPosition (the only CScriptThing-vector global left in
//   the name set with a vector and a C3DVector).
// Callees: CScriptGameResourceObjectScriptedThingBase() @ 0x007E72A0, its MoveToPosition(pos, speed, type, bool, bool)
//   @ 0x007E72F0 (virtual slot 4, called directly on the local), its destructor @ 0x007E74D0; CScriptThing slot 75
//   IsAlive (+0x12c); CGameScriptInterface (global @ 0x0143E8F8) slot 8 StartScriptingEntity (+0x20, 0x0089B5B0) with
//   priority 4.
// Shape: frame-pointer size TU; one resource-object local for the whole loop (built before begin is read, destroyed
//   after it), begin read once into esi, end re-read every pass, the position kept in ebx; the speed is the float 0
//   (`fldz ; push ecx ; fstp [esp]`), the move type 0, the flags false / true.
class C3DVector { public:
    float X, Y, Z;
};
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
enum EScriptEntityMoveType { MOVE_TYPE_WALK = 0 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
class CScriptGameResourceObjectScriptedThingBase {      // 0x10 bytes, vtable 0x0127094C
public:
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    void MoveToPosition(const C3DVector& position, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
    unsigned char _body[0x10];
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007();
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

void __fastcall MoveAllThingsInVectorToPosition(const std::vector<CScriptThing>& things, const C3DVector& position)
{
    CScriptGameResourceObjectScriptedThingBase object;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it) {
        if (it->IsAlive()) {
            g_pGameScriptInterface->StartScriptingEntity(*it, object, SCRIPT_AI_PRIORITY_4);
            object.MoveToPosition(position, 0.0f, MOVE_TYPE_WALK, false, true);
        }
    }
}