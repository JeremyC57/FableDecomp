# h26_c693.py: naming wave 26, family c693 -- the chunk 693 queue rows whose own row still spells a stand-in:
#  0x00E241B0 ??1CIsThingAlive@NScript@@UAE@XZ (was CScriptThingFunctor_00e241b0), 0x009CBFE0 ?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ
#  (private: was QAE), 0x00994400 ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z (was a CDiskFileWin32* ctor), 0x009CDA70
#  ?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z (was CBankFile_Retail). 0x00994480 Write stays deferred (its UAE
#  virtual needs a vptr the engine CFileDataOutputStream.h header flattens: DECISIONS_W26).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he

# --- 0x00E241B0 ~CIsThingAlive
p, rel, t = he._rw('00e241b0', 'src')
if 'CScriptThingFunctor_00e241b0' in t:
    t = t.replace('''// CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0() -- this class's vtable (0x012C32F0)''',
                  '''// NScript::CIsThingAlive::~CIsThingAlive() -- this class's vtable (0x012C32F0)''')
    t = t.replace('// Leaf: ??1CScriptThingFunctor_00e241b0@@QAE@XZ\n', '''// Leaf: ??1CIsThingAlive@NScript@@UAE@XZ
// Identity (naming wave 26, c693 queue): Ego_r.pdb + FableWin.pdb ??1CIsThingAlive@NScript@@UAE@XZ (labels_naming_wave9 / labels_chunk571);
//   engine/CIsThingAlive.h (vtable 0x012C32F0: operator() 0x00CDF120, Clone 0x00CDF130, scalar deleting dtor 0x00CDF160 -> this body); the
//   final store is NScript::CPredicateBase's vtable 0x012C2FF4 (engine/CPredicateBase.h, inline destructor). Was
//   CScriptThingFunctor_00e241b0 (non-virtual ??1CScriptThingFunctor_00e241b0@@QAE@XZ) over a CThingFunctorBase_012c2ff4 stand-in base. The
//   CScriptThing member keeps the row's local model (its destructor expanded inline here, as retail; engine/CScriptThing.h declares it
//   out of line).
''')
    t = t.replace('// The chunk label "CIsThingAlive::~CIsThingAlive" is shared by two functors with different vtables; named by address. Shape: `this` in edi, the member in esi.\n',
                  '// Shape: `this` in edi, the member in esi.\n')
    old = t[t.index('struct CThingFunctorBase_012c2ff4 {'):]
    t = t.replace(old, '''}   // namespace NScript (closed for the shared header's include guard; reopened below)
''') if False else t.replace(old, '''namespace NScript {
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0 (engine/CIsThingAlive.h; CScriptThing local, see above)
public:
    virtual bool operator()() const;                    // slot 0  0x00CDF120
    virtual CPredicateBase* Clone() const;              // slot 1  0x00CDF130
    virtual ~CIsThingAlive();                           // slot 2  0x00CDF160 (scalar deleting)
    CScriptThing Thing;                                 // +0x04
};
}

NScript::CIsThingAlive::~CIsThingAlive()
{
}
''')
    t = t.replace('void __cdecl operator delete(void* block);         // 0x00BFE9BC\n',
                  'void __cdecl operator delete(void* block);         // 0x00BFE9BC\n#define FABLE_RETAIL_NSCRIPT\n#include "engine/CPredicateBase.h"                    // NScript::CPredicateBase, vtable 0x012C2FF4\n', 1)
    he._w(p, t); print('ok 00e241b0 src')
else: print('already 00e241b0 src')

# --- 0x009CBFE0 CBankFile::GetRecoveryFilePath is private (Ego_r.pdb / FableWin.pdb AAE)
he.edit('009cbfe0', 'src', '''class CBankFile : public CBankFileDonor {
public:
    CWideString GetRecoveryFilePath();
};''', '''class CBankFile : public CBankFileDonor {
private:
    CWideString GetRecoveryFilePath();                 // Ego_r.pdb / FableWin.pdb ?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ (private)
};''')
he.edit('009cbfe0', 'src', '// Leaf: GetRecoveryFilePath\n', '''// Leaf: ?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ
// Naming wave 26 (c693 queue): private, as Ego_r.pdb / FableWin.pdb spell it (sole caller CBankFile::WriteRecoveryFile 0x009CDF50, landed by
//   chunk 693 on that spelling); was public (QAE).
''')

# --- 0x00994400 CFileDataOutputStream::CFileDataOutputStream(CAFile&) (Ego_r.pdb + FableWin.pdb ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z)
he.edit('00994400', 'src', '''struct CFileDataOutputStream : CDataOutputStream { // vtable 0x0129A6F8
    void* m_pFile;                                 // +0x08
    CFileDataOutputStream(void* file);
};

CFileDataOutputStream::CFileDataOutputStream(void* file)
    : CDataOutputStream()
    , m_pFile(file)
{
}''', '''class CAFile;
class CFileDataOutputStream : public CDataOutputStream { public:   // vtable 0x0129A6F8
    CAFile* File;                                  // +0x08
    CFileDataOutputStream(CAFile& file);
};

CFileDataOutputStream::CFileDataOutputStream(CAFile& file)
    : CDataOutputStream()
    , File(&file)
{
}''')
he.edit('00994400', 'src', '''// CFileDataOutputStream::CFileDataOutputStream(void* file) -- ''', '''// CFileDataOutputStream::CFileDataOutputStream(CAFile& file) -- ''')
he.edit('00994400', 'src', '// Leaf: ??0CFileDataOutputStream@\n', '''// Leaf: ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z
// Naming wave 26 (c693 queue): Ego_r.pdb + FableWin.pdb ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z -- the file by reference (the same
//   pointer push); the member is the PDB's File (engine/CFileDataOutputStream.h +0x08). Was CFileDataOutputStream(void*) on m_pFile.
''')

# --- 0x009CDA70 CBankFile::InternalWriteEntryList: the local CBankFile_Retail model is CBankFile (Ego_r.pdb / FableWin.pdb
#     ?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z; chunk 693's WriteRecoveryFile 0x009CDF50 binds it)
p, rel, t = he._rw('009cda70', 'src')
if 'CBankFile_Retail' in t:
    t = t.replace('struct CBankFile_Retail {', 'class CBankFile { public:          // the landed retail CBankFile model (local: engine/CBankFile.h migration pending)')
    t = t.replace('CBankFile_Retail::', 'CBankFile::')
    assert 'CBankFile_Retail' not in t
    t = t.replace('// Leaf: InternalWriteEntryList\n', '''// Leaf: ?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z
// Naming wave 26 (c693 queue): Ego_r.pdb + FableWin.pdb ?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z (the spelling chunk
//   693's CBankFile::WriteRecoveryFile 0x009CDF50 binds); the local layout model was named CBankFile_Retail.
''')
    he._w(p, t); print('ok 009cda70 src')
else: print('already 009cda70 src')
