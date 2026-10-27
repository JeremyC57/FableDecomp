// NScript::CIsThingConscious::Clone() const -- a new heap copy of the predicate (0 when the allocation fails)
// Leaf: ?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDDAE0, 48 bytes, thiscall, plain `ret` (virtual slot 1).
// Identity (proven): RTTI-less vtable 0x012C2FE8: slot 0 0x00CDDAB0, slot 1 0x00CDDAE0, slot 2 the landed ??_GCIsThingConscious@NScript@@
//   0x00CDDB10; Ego_r.pdb NScript::CIsThingConscious has exactly operator(), Clone and the destructor (??RCIsThingConscious@NScript@@UBE_NXZ,
//   ?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ); its constructor from a CScriptThing is in Ego_r.pdb too.
// Layout: CPredicateBase vptr, CScriptThing Thing +0x04 (0x10 bytes; the model is the landed 0x00CDDB10 one).
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the slot 61 / 75 calls need a polymorphic local model.
// Callees: scalar operator new 0x00BFEA1A (0x10 bytes), CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90.
// Shape: the implicit copy constructor inlined: only the class's own vtable 0x012C2FE8 is stored, before the Thing copy.
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual bool IsUnconscious() const;                 // slot 61 (+0xf4) 0x004AAFA0
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    unsigned long Data[2];
};
namespace NScript {
class CPredicateBase {                                  // vtable 0x012C2FF4 (slots 0 / 1 pure)
public:
    CPredicateBase() { }
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingConscious : public CPredicateBase {       // vtable 0x012C2FE8
public:
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;                    // slot 0, 0x00CDDAB0
    virtual CPredicateBase* Clone() const;              // slot 1, 0x00CDDAE0
    virtual ~CIsThingConscious() { }                    // slot 2, ??_G 0x00CDDB10 (landed)
    CScriptThing Thing;                                 // +0x04
};
}

NScript::CPredicateBase* NScript::CIsThingConscious::Clone() const
{
    return new CIsThingConscious(*this);
}