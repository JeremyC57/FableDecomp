import re
from he import edit, path
def sub_all(a, k, pat, rep):
    p, rel = path(a, k); t = open(p, newline='').read()
    m = re.findall(pat, t)
    t2 = re.sub(pat, rep, t)
    open(p, 'w', newline='').write(t2); print('sub', a, k, len(m))
RA = '// 0x0099A2D0 ??0CBaseClass@@QAE@XZ'
# 0054ea00: NUISystem::CBase (slot-0 placeholder + ctor) -> file-scope CBaseClass (slot 0 = its virtual dtor)
edit('0054ea00', 'src', 'struct CBase {\n    virtual void v00();\n    CBase();                                   // 0x0099A2D0\n};\n\nstruct CEnginePrimitiveHandle : CBase {        // vtable 0x0122F598\n    void* m_pPrimitive;                        // +4\n    virtual void v00();\n    CEnginePrimitiveHandle() : CBase(), m_pPrimitive(0) { }\n',
     '}   // the retail root is file scope\nclass CBaseClass { public:\n    virtual ~CBaseClass();                     // slot 0 (0x0099A2E0)\n    CBaseClass();                              // 0x0099A2D0 ??0CBaseClass@@QAE@XZ\n};\nnamespace NUISystem {\n\nstruct CEnginePrimitiveHandle : CBaseClass {   // vtable 0x0122F598\n    void* m_pPrimitive;                        // +4\n    CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) { }\n')
edit('0054ea00', 'src', 'CBase::CBase @ 0x0099A2D0', 'CBaseClass::CBaseClass @ 0x0099A2D0')
# 00769e20: N00769e20::CBase (virtual dtor) -> file-scope CBaseClass
edit('00769e20', 'src', 'namespace N00769e20 {\nstruct CBase {\n    virtual ~CBase();                                     // 0x0099A2E0 (restores the base vtable)\n};\n',
     'class CBaseClass { public:\n    virtual ~CBaseClass();                                // 0x0099A2E0 ??1CBaseClass@@UAE@XZ (restores the base vtable)\n};\nnamespace N00769e20 {\n')
edit('00769e20', 'src', 'struct CParticleUpdateParamDesc : CBase {', 'struct CParticleUpdateParamDesc : CBaseClass {')
# 00af4530: NParticleEngine::CBaseClass -> file scope
edit('00af4530', 'src', 'class CBaseClass {\npublic:\n    CBaseClass();                                  // 0x0099A2D0\n    virtual ~CBaseClass();                         // 0x0099A2E0\n};\n',
     '}   // the retail root is file scope\nclass CBaseClass {\npublic:\n    CBaseClass();                                  // 0x0099A2D0 ??0CBaseClass@@QAE@XZ\n    virtual ~CBaseClass();                         // 0x0099A2E0 ??1CBaseClass@@UAE@XZ\n};\nnamespace NParticleEngine {\n')
# 0085d370: NThingMap::CBaseClassNonCopyable (protected ctor + slot-0 placeholder) -> file scope, slot 0 = protected virtual dtor
edit('0085d370', 'src', 'struct CBaseClassNonCopyable {\nprotected:\n    CBaseClassNonCopyable();                       // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)\npublic:\n    virtual void v00();\n};\n',
     '}   // the retail root is file scope\nclass CBaseClassNonCopyable {\nprotected:\n    CBaseClassNonCopyable();                       // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)\n    virtual ~CBaseClassNonCopyable();              // slot 0 (0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ)\n};\nnamespace NThingMap {\n')
edit('0085d370', 'src', '    CThingMap(CThingMapManager* manager, const C2DBoxI& area);\n    virtual void v00();\n', '    CThingMap(CThingMapManager* manager, const C2DBoxI& area);\n')
# 009ee180: FableCons_009ee180::CBaseClass (slot-0 placeholder + ctor) -> file scope
edit('009ee180', 'src', 'struct CBaseClass {\n    virtual void v00();                                         // vtable 0x01231710\n    CBaseClass();                                               // 0x0099A2D0\n};\n',
     '}   // the retail root is file scope\nclass CBaseClass { public:\n    virtual ~CBaseClass();                                      // slot 0; vtable 0x01231710\n    CBaseClass();                                               // 0x0099A2D0 ??0CBaseClass@@QAE@XZ\n};\nnamespace FableCons_009ee180 {\n')
edit('009ee180', 'src', '    long        CursorPosition;                                 // +0x0c\n    virtual void v00();\n', '    long        CursorPosition;                                 // +0x0c\n')
# 0057cdb5: CPrimitiveHandleBase (slot-0 placeholder + ctor) -> CBaseClass
edit('0057cdb5', 'src', 'struct CPrimitiveHandleBase {\n    virtual void h00();\n    CPrimitiveHandleBase();                          // 0x0099A2D0\n};\nclass CEnginePrimitiveHandle : public CPrimitiveHandleBase { public:   // vtable 0x0122F598\n    long Handle;                                     // +0x04\n    __forceinline CEnginePrimitiveHandle() : Handle(0) {}\n    virtual void h00();\n',
     'class CBaseClass { public:\n    virtual ~CBaseClass();                           // slot 0 (0x0099A2E0)\n    CBaseClass();                                    // 0x0099A2D0 ??0CBaseClass@@QAE@XZ\n};\nclass CEnginePrimitiveHandle : public CBaseClass { public:   // vtable 0x0122F598\n    long Handle;                                     // +0x04\n    __forceinline CEnginePrimitiveHandle() : Handle(0) {}\n')
# 00b9ab30 / 00baba70: CPrimitiveHandleBaseNative -> CBaseClass
for a in ('00b9ab30', '00baba70'):
    sub_all(a, 'src', r'\bstruct CPrimitiveHandleBaseNative \{', 'class CBaseClass { public:')
    sub_all(a, 'src', r'\bCPrimitiveHandleBaseNative\b', 'CBaseClass')
