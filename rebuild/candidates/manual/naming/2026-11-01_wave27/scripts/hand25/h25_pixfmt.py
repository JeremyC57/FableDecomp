# h25_pixfmt.py: naming wave 25, family pixfmt -- the binders of ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z 0x009E3830 (c692 queue;
#  Ego_r.pdb CPixelFormat::Initialise(_D3DFORMAT); the body ends without `mov eax,ecx`, so it is no constructor: the "CPixelFormat(format)"
#  sites are an inline constructor that calls it, as the landed 0x009F37E0 / 0x009F4190 already model).
#  Every row's local CPixelFormat now declares `void Initialise(_D3DFORMAT)` (was long / unsigned long / int / a D3DFORMAT typedef) and,
#  where the row builds a CPixelFormat from a format, the inline `CPixelFormat(_D3DFORMAT f) { Initialise(f); }` (was an out-of-line
#  ??0CPixelFormat@@QAE@J@Z / @K@Z); the stand-in classes CPixelFormatFromD3D / CPixelFormatNative / CPixelFormat_Methods / PixelFormat /
#  PixelFormatMethods / CGlowPixelFormat / CPixelFormatByteLengthView become CPixelFormat. The format arguments are D3DFORMAT values
#  (the D3D desc Format fields, the D3DFMT_* constants), passed as before: one pushed dword, the bytes unchanged.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
ENUM = ('enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844,'
        ' D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)\n')
NOTE = ('// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:\n'
        '//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.\n')
def note(t):
    if 'Naming wave 25: CPixelFormat::Initialise' in t: return t
    L = t.split('\n'); i = next((j for j, x in enumerate(L) if x.startswith('// Leaf: ')), -1)
    L.insert(i + 1 if i >= 0 else 1, NOTE.rstrip('\n')); return '\n'.join(L)
def gen(a, extra=()):
    p, rel, t = he._rw(a, 'src'); t0 = t
    for old, new in extra:
        if old not in t and new in t: continue
        assert t.count(old) >= 1, (a, old)
        t = t.replace(old, new)
    if not re.search(r'enum _D3DFORMAT\b', t):
        L = t.split('\n'); i = next(j for j, x in enumerate(L) if not x.startswith('//'))
        L.insert(i, ENUM.rstrip('\n')); t = '\n'.join(L)
    # the D3D description / mode / info Format fields are D3DFORMAT values
    t = re.sub(r'\b(?:unsigned long|long|unsigned int|int|D3DFORMAT)(\s+)(Format|D3DFormat|BackBufferFormat|format00);', r'_D3DFORMAT\1\2;', t)
    t = t.replace('    long Format, Type;', '    _D3DFORMAT Format; long Type;')
    # the D3DFMT_* literals the rows pass (their comments name them)
    for lit, nm in (('0x35545844', 'D3DFMT_DXT5'), ('0x15', 'D3DFMT_A8R8G8B8'), ('0x4b', 'D3DFMT_D24S8')):
        t = re.sub(r'(Initialise|CPixelFormat|format|Format)\(' + lit + r'\)', r'\1(' + nm + ')', t)
    t = t.replace('best.Format = 0;', 'best.Format = D3DFMT_UNKNOWN;')
    t = re.sub(r'void Initialise\((?:long|unsigned long|int|unsigned int|unsigned) ?(\w*)\);', lambda m: 'void Initialise(_D3DFORMAT %s);' % (m.group(1) or 'format'), t)
    t = re.sub(r'CPixelFormat\((?:long|unsigned long) (\w+)\) \{ Initialise\(\1\); \}', r'CPixelFormat(_D3DFORMAT \1) { Initialise(\1); }', t)
    t = re.sub(r'CPixelFormat\((?:long|unsigned long) (\w+)\);( *// 0x009E3830[^\n]*)', r'CPixelFormat(_D3DFORMAT \1) { Initialise(\1); }\n    void Initialise(_D3DFORMAT \1);\2', t)
    t = note(t)
    if t != t0: he._w(p, t); print('ok', a, 'src')
    else: print('already', a, 'src')
A_B_C = '''00416c8a 0042ddb3 0071d3b0 009be590 009be610 009be6c0 009be750 009f4120 009fa340 009fbbd0 00a674e0
009bed10 009bf600 009bfb30 009f37e0 009f4190 00a67860 00a67950 00a67a20
009bebb0 009f2fc0 009f3390 009f35d0 009f9e80 00a681f0 00b86a70 00babb50'''.split()
EXTRA = {}
exec(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'h25_pixfmt_extra.py')).read()) if os.path.exists(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'h25_pixfmt_extra.py')) else None
WP = [('    void WritePixel(const CRGBColour& colour, void* dest) const;   // 0x009E3F60',
       '    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60 (Ego_r.pdb; naming wave 25, was WritePixel)'),
      ('format.WritePixel(colour, ', 'format.ConvertRGBToPFDData(colour, ')]
EXTRA['009f4120'] = WP; EXTRA['009f4190'] = WP
for a in A_B_C:
    gen(a, EXTRA.get(a, ()))

# ---- the stand-in classes (group D) ----
INL = 'CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }\n    void Initialise(_D3DFORMAT d3dFormat);'
LAYOUT = ('#define CPixelFormat CPixelFormatLayout\n#include "engine/CPixelFormat.h"\n#undef CPixelFormat\n')
def fromd3d(a):
    # CPixelFormatFromD3D: the PDB class CPixelFormat with its inline format constructor (the row's GetPixelFormat returns it by value)
    p, rel, t = he._rw(a, 'src')
    if 'CPixelFormatFromD3D' not in t: print('already', a, 'src'); return
    t = re.sub(r'struct CPixelFormatFromD3D \{', 'class CPixelFormat { public:', t)
    t = re.sub(r'CPixelFormatFromD3D\(unsigned long d3dFormat\);( *// 0x009E3830[^\n]*)', INL + r'\1', t)
    t = t.replace('CPixelFormatFromD3D', 'CPixelFormat')
    he._w(p, t); gen(a)
for a in ('009f2f60', '009f9d80', '00a67480'):   # 0x009F31C0 deferred: its WriteColour takes CRGBColour by value (no PDB spelling)
    fromd3d(a)
gen('00b27ee0', [('#include "engine/CPixelFormat.h"\n', LAYOUT),
                 ('''struct CPixelFormatNative : CPixelFormat {
    CPixelFormatNative(long d3dFormat);            // 0x009E3830
};''', '''class CPixelFormat : public CPixelFormatLayout { public:   // was a CPixelFormatNative stand-in
    ''' + INL + '''   // 0x009E3830
};'''),
                 ('CPixelFormatNative format(0x4b);', 'CPixelFormat format(D3DFMT_D24S8);')])
gen('009f3ff0', [('''    unsigned long GetByteColourDepth() const;   // 0x009E3800
};''', '''    unsigned long GetByteColourDepth() const;   // 0x009E3800
    void Initialise(_D3DFORMAT d3dFormat);      // 0x009E3830 (naming wave 25: was on a CPixelFormat_Methods stand-in)
    void ConvertRGBToPFDData(const class CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60 (Ego_r.pdb; was on the stand-in)
};'''), ('''struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(unsigned long d3dFormat);      // 0x009E3830 (D3DFORMAT)
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60
};
''', ''), ('    CPixelFormat_Methods format;', '    CPixelFormat format;')])
for a in ('009f3470', '009f3520'):
    gen(a, [('''    unsigned long GetByteColourDepth() const;   // 0x009E3800
};''', '''    unsigned long GetByteColourDepth() const;   // 0x009E3800
    void Initialise(_D3DFORMAT d3dFormat);      // 0x009E3830 (naming wave 25: was on a CPixelFormat_Methods stand-in)
};'''), ('''struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(unsigned long d3dFormat);      // 0x009E3830 (D3DFORMAT)
};
''', ''), ('    CPixelFormat_Methods format;', '    CPixelFormat format;')])
gen('009fcef0', [('#include "engine/CPixelFormat.h"\n', LAYOUT),
                 ('struct PixelFormat {fable_i32 FormatIndex;void Initialise(long,long,long,long,bool);void Initialise(_D3DFORMAT);};',
                  'class CPixelFormat { public: fable_i32 FormatIndex;void Initialise(long,long,long,long,bool);void Initialise(_D3DFORMAT);};'),
                 ('struct PixelFormats {PixelFormat NonAlpha,', 'struct PixelFormats {CPixelFormat NonAlpha,')])
gen('00b5bf10', [('#include "engine/CPixelFormat.h"\n#else', LAYOUT + '#else'),
                 ('struct PixelFormatMethods : CPixelFormat { PixelFormatMethods(long f) { FormatIndex=-1; Initialise(f); } void Initialise(long); }; // 009e3830',
                  '#ifdef _MSC_VER\nclass CPixelFormat : public CPixelFormatLayout { public: CPixelFormat() { FormatIndex=-1; } void Initialise(_D3DFORMAT); }; // 009e3830 (was a PixelFormatMethods stand-in)\n#endif'),
                 (' PixelFormatMethods format(21);', ' CPixelFormat format; format.Initialise(D3DFMT_A8R8G8B8);')])
gen('00b6d510', [('#include "engine/CPixelFormat.h"\n#endif', LAYOUT + '#endif'),
                 ('struct PixelFormatMethods:CPixelFormat {void Initialise(int);}; //009e3830 D3DFORMAT enum passed as32bit int',
                  '#ifdef _MSC_VER\nclass CPixelFormat : public CPixelFormatLayout { public: void Initialise(_D3DFORMAT); }; //009e3830 (was a PixelFormatMethods stand-in)\n#endif'),
                 (' PixelFormatMethods format;format.Initialise(21);', ' CPixelFormat format;format.Initialise(D3DFMT_A8R8G8B8);')])
gen('00b5ece0', [('#include "engine/CPixelFormat.h"\n', LAYOUT),
                 ('struct CGlowPixelFormat:CPixelFormat {CGlowPixelFormat(){FormatIndex=-1;}void Initialise(unsigned format);}; //009e3830',
                  'class CPixelFormat : public CPixelFormatLayout { public: CPixelFormat(){FormatIndex=-1;}void Initialise(_D3DFORMAT format);}; //009e3830 (was a CGlowPixelFormat stand-in)'),
                 (' CGlowPixelFormat format;format.Initialise(21);', ' CPixelFormat format;format.Initialise(D3DFMT_A8R8G8B8);')])
gen('00b3b4f0', [('enum D3DFORMAT { D3DFMT_DXT1 = 0x31545844 };', 'enum _D3DFORMAT { D3DFMT_DXT1 = 0x31545844 };\ntypedef _D3DFORMAT D3DFORMAT;')])

# ---- 0x00A681F0 (c692 queue): ?GetLevelByteLength@CVolumeTexture@@QBEJJ@Z (Ego_r.pdb member GetLevelByteLength(long) const; called per
#      level from CVolumeTexture::SaveToDataStream 0x00A68AE0); was GetByteLength(unsigned long) returning unsigned long ----
gen('00a681f0', [('''// CVolumeTexture::GetByteLength(unsigned long level) -- size in bytes''', '''// CVolumeTexture::GetLevelByteLength(long level) -- size in bytes'''),
                 ('''    unsigned long GetByteLength(unsigned long level) const;
};

unsigned long CVolumeTexture::GetByteLength(unsigned long level) const''', '''    long GetLevelByteLength(long level) const;
};

long CVolumeTexture::GetLevelByteLength(long level) const''')])
# ---- rows without a Leaf line (graded on their largest function): the Leaf is the row's own current spelling ----
def leaf(a, sym, extra=None):
    p, rel, t = he._rw(a, 'src')
    if '\n// Leaf: ' in '\n' + t: print('already', a, 'src'); return
    L = t.split('\n'); L.insert(1, '// Leaf: ' + sym)
    if extra: L.insert(2, extra)
    he._w(p, '\n'.join(L)); print('ok', a, 'src')
for a, sym in (('00416c8a', '?InitGraphics@CMainGameComponent@@QAE_NXZ'), ('0042ddb3', '?InitialiseGraphics@CNewFrontendGameComponent@@QAE_NXZ'),
               ('009be590', '?GetBestSignedTextureFormat@CDisplayManager@@QAE_NJAAVCPixelFormat@@@Z'),
               ('009be6c0', '?GetBestAlphaTextureFormat@CDisplayManager@@QAE_NJAAVCPixelFormat@@@Z'), ('009f4120', '?SetPixel@CSurface@@QAEXJJABVCRGBColour@@@Z'),
               ('009bed10', '?GetCurrentWindowedSettings@CDisplayManager@@QAEXPAUHWND__@@AAVC2DExtentsI@@AAK@Z'),
               ('009f2fc0', '?GetByteLength@CSurface_Methods@@QBEKXZ'), ('009f3390', '?GetPitch@CSurface_Methods@@QBEKXZ'),
               ('009f9e80', '?GetLevelByteLength@CTexture@@QBEKK@Z'), ('00a681f0', '?GetLevelByteLength@CVolumeTexture@@QBEJJ@Z'),
               ('00b86a70', '?InitialiseTextures@CEngineScreenEffectDisplacementRenderer_Init@@QAE_NXZ'),
               ('009f2f60', '?GetPixelFormat@CSurface_Methods@@QBE?AVCPixelFormat@@XZ'),
               ('009f9d80', '?GetPixelFormat@CTexture_Methods@@QAE?AVCPixelFormat@@XZ'), ('00a67480', '?GetPixelFormat@CVolume@@QBE?AVCPixelFormat@@XZ'),
               ('00b27ee0', '?GetScratchZBufferD24S8@CEngine@@QAEPAUCSurfaceNative@@K@Z'), ('00b5bf10', '?InitialiseTextures@RadialBlurMethods@@QAEJXZ'),
               ('00b6d510', '?InitialiseDynamicTextures@CWaterDynamicTextures@@QAEXXZ'), ('00b5ece0', '?InitialiseTextures@CGlowRendererMethods@@QAEHXZ'),
               ('00b3b4f0', '?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z')):
    leaf(a, sym)
