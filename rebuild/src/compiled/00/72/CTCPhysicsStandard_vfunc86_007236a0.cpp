// CTCPhysicsStandard::vfunc_86() -- when a physics mesh info is attached, passes its first word to the virtual at slot 85
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007236A0, length 20 bytes (vtable 0x01265F84 slot 86, rtti-vtables.tsv).
// Callees: this class's virtual slot 85 (`call [edx+154h]`), argument pushed as a zero-extended word.
// Shape: `mov eax,[ecx+40h] ; test ; je ; movzx eax,word [eax] ; mov edx,[ecx] ; push eax ; call [edx+154h] ; ret` -- the
// virtual call is the last statement but is not tail-jumped (a virtual call never is); the word argument widens to a long
// parameter with `movzx` (unsigned short).
// Layout: Ego_r.pdb CTCPhysicsStandard PPhysicsMeshInfo at +0x40; the mesh info's first word is modelled as m_type (descriptive).
// engine/CTCPhysicsStandard.h has no virtuals, so the class is modelled locally.
struct CThingPhysicsMeshInfo {
    unsigned short m_type;                     // +0x00
};

struct CTCPhysicsStandard {
    virtual void v00();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void vfunc_85(long meshType);      // slot 85, +0x154
    virtual void vfunc_86();                   // slot 86
    unsigned char _base[0x3c];                 // +0x04 CTCPhysicsBase body
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;   // +0x40
};

void CTCPhysicsStandard::vfunc_86()
{
    if (PPhysicsMeshInfo)
        vfunc_85(PPhysicsMeshInfo->m_type);
}