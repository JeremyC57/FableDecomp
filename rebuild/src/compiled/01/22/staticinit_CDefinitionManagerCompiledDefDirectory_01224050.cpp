// dynamic initializer for CWideString CDefinitionManager::CompiledDefDirectory @ 0x013CA7DC -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224050, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13ca7dc ; call 0x99aed0 ; push 0x122a6b0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013CA7DC: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A6B0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER_COMPILEDDEFDIR
#include "engine/CDefinitionManager.h"   // opt-in: protected static CompiledDefDirectory (agentFAM09 hdr overlay)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CDefinitionManager::CompiledDefDirectory'' (0x0101ABB0) +
//   FableWin.pdb (0x0400AEB0); Ego_r runs NBankFileManager::GBankFileManager, SymbolHeaderDir ..
//   CDefinitionManager::CompiledDefDirectory, then three singleton/font PClass statics and GDefStringTable = retail 0x01224010,
//   0x01224030 (landed), this (default CWideString), 0x01224070 .. 0x01224090 (atexit-only), 0x012240A0 (landed). Anniv360 Test
//   PDB (third source): ?CompiledDefDirectory@CDefinitionManager@@1VCWideString@@A. Was g_CCharString_013ca7dc.
CWideString CDefinitionManager::CompiledDefDirectory;    // 0x013CA7DC ?CompiledDefDirectory@CDefinitionManager@@1VCWideString@@A
