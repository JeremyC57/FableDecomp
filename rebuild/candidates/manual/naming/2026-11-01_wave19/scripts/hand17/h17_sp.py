# h17_sp.py: naming wave 17, wave 16 DEFERRED 00404720 -- the CStringParser ctor's own row. Its behaviour test printed
# "00404720_TEST PASS" while the row's PassPattern is STRINGPARSER_CTOR_OK (why the HEAD recheck FAILed: the test logic passes);
# the test now prints the pattern too. The row is respelled on the proven ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z
# (wave 16 sp: the __fastcall callback taking const CCharString&, as its 33 callers already are) and Ego_r's char Init
# ?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z (const char*). Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
a = '00404720'
edit(a, 'src', '// CStringParser::CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset) -- build a parser over a CCharString\n// Leaf: ??0CStringParser\n',
     '// CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset) -- build a parser over a CCharString\n'
     '// Leaf: ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z\n'
     '// Naming wave 17: the callback is the __fastcall (const CCharString&) one its 33 callers name (wave 16 sp) and Init is Ego_r\'s\n'
     '//   ?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z (const char*); was void (*)(CCharString*) / char* -- name-only.\n')
edit(a, 'src', 'typedef void (*StringParserErrorCallback)(CCharString*);', 'typedef void (__fastcall* StringParserErrorCallback)(const CCharString&);')
edit(a, 'src', '    void Init(char* text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530',
     '    void Init(const char* text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530')
edit(a, 'tst', '    std::printf(ok ? "00404720_TEST PASS\\n" : "00404720_TEST FAIL\\n"); return ok ? 0 : 1;',
     '    // the row\'s PassPattern is STRINGPARSER_CTOR_OK (naming wave 17: the test printed only the generic line)\n'
     '    std::printf(ok ? "STRINGPARSER_CTOR_OK\\n00404720_TEST PASS\\n" : "00404720_TEST FAIL\\n"); return ok ? 0 : 1;')
# 0x009AE150 CDefinitionManager::CompileSubDefinition: the CStringParser ctor callback and the CPersistContext ctor on their PDB spellings
#   (??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z; Ego_r ??0CPersistContext@@QAE@AAVCStringParser@@P6IXABVCCharString@@@Z_NPBVCDefinitionManager@@PBVCSymbolMap@@PAV2@@Z).
a = '009ae150'
edit(a, 'src', 'typedef void (*TErrorCallback)(CCharString*);',
     'typedef void (__fastcall* TErrorCallback)(const CCharString&);   // naming wave 17: the PDB callback type (was void (*)(CCharString*))')
edit(a, 'src', '''    CPersistContext(CStringParser* parser, TErrorCallback callback, bool isNew, CDefinitionManager* manager,
                    CSymbolMap* symbols, CCharString* text);  // 0x009BADD0''', '''    CPersistContext(CStringParser& parser, TErrorCallback callback, bool isNew, const CDefinitionManager* manager,
                    const CSymbolMap* symbols, CCharString* text);  // 0x009BADD0 (Ego_r signature; naming wave 17)''')
edit(a, 'src', '        CPersistContext context(&parser, ErrorCallback, isNew, this, PSymbolMap, &text);',
     '        CPersistContext context(parser, ErrorCallback, isNew, this, PSymbolMap, &text);')
