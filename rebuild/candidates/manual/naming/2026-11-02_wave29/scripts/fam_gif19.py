# fam_gif19.py <ov> <rows.txt> [tag]: CGraphicInfoFrame::CFrameData -- the frame-data class is nested in CGraphicInfoFrame (Ego_r.pdb
# ??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z, ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z, RTTI .?AVCFrameData@CGraphicInfoFrame@@)
# and its constructor takes a const CGraphicInfoFrame*; InternalGetFrameForcedLoading is the private const member
# ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z. The landed rows model a global CFrameData (c684 queue rows 0x009FE380 /
# 0x009FF6B0). Name-only respell: the CFrameData definition becomes `class CGraphicInfoFrame::CFrameData` after CGraphicInfoFrame (which
# forward-declares it as a nested class); the row's pointer helper (CFrameDataPointer / ResourcePointer) that holds a CFrameData* before
# CGraphicInfoFrame is complete becomes a template on the pointee (CIVCountedPointer<CFrameData> shape); a global typedef keeps the
# unqualified name for the row's free code. check: the old spellings are gone, the nested ones named, nothing else changes.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, acclib
import fam_dtor_set as FS
from memlib import depth_at
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'gif'
CTOR = '??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z'; IGF = '?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z'
IGF_DECL = 'bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0 (Ego_r.pdb ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z, private)'
def stmt_span(c, pos):
    j = pos
    while j > 0 and c[j - 1] not in ';{}:': j -= 1
    while j < pos and c[j] in ' \t\n': j += 1
    return j, c.index(';', pos) + 1
def tr(t, a):
    c = F.code_only(t)
    fd = F.class_defs(c, 'CFrameData')
    if len(fd) != 1: return None, '%d definitions of CFrameData' % len(fd)
    gf = F.class_defs(c, 'CGraphicInfoFrame')
    if len(gf) != 1: return None, '%d definitions of CGraphicInfoFrame' % len(gf)
    kwf, sf, bf, ef = fd[0]; kwg, sg, bg, eg = gf[0]
    if sf > sg: return None, 'CFrameData defined after CGraphicInfoFrame'
    if depth_at(c, sf) or depth_at(c, sg): return None, 'nested in a namespace / class'
    if re.search(r'CGraphicInfoFrame::CFrameData', c): return None, 'already nested'
    # the frame-data block
    hdr = t[sf:bf]; body = t[bf + 1:ef]; endf = c.index(';', ef) + 1
    m = re.match(r'\s*(class|struct)\s+CFrameData\s*(:\s*(?:public\s+)?(\w+))?\s*$', hdr)
    if not m: return None, 'CFrameData header not understood: %r' % hdr[:60]
    base = m.group(3)
    body = re.sub(r'\bCFrameData\s*\(\s*CGraphicInfoFrame\s*\*', 'CFrameData(const CGraphicInfoFrame*', body)
    if 'CFrameData(const CGraphicInfoFrame*' not in body: return None, 'no CFrameData(CGraphicInfoFrame*) constructor in the body'
    # tail comment on the header line
    le = t.find('\n', bf); tailc = t[ef + 1:t.find('\n', ef) + 1] if t[ef + 1:t.find('\n', ef)].strip().startswith(';') else ''
    nested = ('class CGraphicInfoFrame::CFrameData' + (' : public %s' % base if base else '') + ' { public:' + body + '};' +
              '   // Ego_r.pdb: nested in CGraphicInfoFrame (RTTI .?AVCFrameData@CGraphicInfoFrame@@), 0x44 bytes\ntypedef CGraphicInfoFrame::CFrameData CFrameData;   // the row\'s free code keeps the unqualified name\n')
    # helpers between the two classes that hold a CFrameData*
    helpers = []
    for mm in re.finditer(r'\b(class|struct)\s+(\w+)\s*\{', c[ef:sg]):
        if mm.group(2) in ('CFrameData',): continue
        hb = ef + mm.end() - 1; he = F.blk_end(c, hb)
        if re.search(r'\bCFrameData\b', c[hb:he]) and not re.search(r'template\s*<[^>]*>\s*$', c[max(0, ef + mm.start() - 40):ef + mm.start()]):
            helpers.append((mm.group(2), ef + mm.start(), hb, he))
    # rewrite: remove the CFrameData block (and the line's remainder), templatize helpers, nested fwd decl, insert the nested def after CGraphicInfoFrame
    out = []; last = 0
    rm_s = t.rfind('\n', 0, sf) + 1 if not t[t.rfind('\n', 0, sf) + 1:sf].strip() else sf
    rm_e = endf + 1 if t[endf:endf + 1] == '\n' else endf
    out.append(t[last:rm_s]); last = rm_e
    for name, hs, hb, he in helpers:
        out.append(t[last:hs]); out.append('template <class T> '); 
        hbody = t[hs:he + 1]
        hbody = F.rename_code(hbody, 'CFrameData', 'T')
        out.append(hbody); last = he + 1
    out.append(t[last:bg + 1]); out.append(' public: class CFrameData;' if kwg == 'class' else ' class CFrameData;')
    gbody = t[bg + 1:eg]
    for name, hs, hb, he in helpers: gbody = F.rename_code(gbody, name, name + '<CFrameData>')
    out.append(gbody); gend = c.index(';', eg) + 1; out.append(t[eg:gend]); last = gend
    rest = t[last:]
    for name, hs, hb, he in helpers: rest = F.rename_code(rest, name, name + '<CFrameData>')
    rest = re.sub(r'(?<![\w:])CFrameData::(\w+)\s*\(', r'CGraphicInfoFrame::CFrameData::\1(', rest)
    t = ''.join(out) + '\n' + nested + rest
    t = re.sub(r'(?m)^(class|struct) CFrameData;[^\n]*\n', '', t)
    # InternalGetFrameForcedLoading: private const
    c = F.code_only(t); gf = [d for d in F.class_defs(c, 'CGraphicInfoFrame') if not re.match(r'\s*(class|struct)\s+CGraphicInfoFrame\s*::', c[d[1]:d[2]])]
    if len(gf) != 1: return None, 'CGraphicInfoFrame lost'
    kwg, sg, bg, eg = gf[0]
    dm = [x for x in re.finditer(r'\bInternalGetFrameForcedLoading\s*\([^;{)]*\)\s*(const\s*)?;', c) if bg < x.start() < eg]
    if len(dm) > 1: return None, '%d InternalGetFrameForcedLoading declarations' % len(dm)
    nI = 0
    if dm:
        ds, de = stmt_span(c, dm[0].start())
        t = FS.set_stmt(t, c, kwg, bg, ds, de, IGF_DECL, 'private'); nI = 1
    t = re.sub(r'CFrameData::CFrameData\(CGraphicInfoFrame\*\)', 'CGraphicInfoFrame::CFrameData::CFrameData(const CGraphicInfoFrame*)', t)
    return t, 'nested CFrameData%s, %d helper(s) templated, %d InternalGetFrameForcedLoading respelled' % ((' : ' + base) if base else '', len(helpers), nI)
def ck(D0, U0, D1, U1, a):
    hz = lambda s: '?A0x' in s
    if CTOR not in U1: return 'nested ctor not named'
    bad = [s for s in U1 if '@CFrameData@@' in s or 'CFrameData@NUISystem' in s]
    if bad: return 'old CFrameData spelling still named %s' % bad[:2]
    pair = lambda s: s.replace('@CFrameData@CGraphicInfoFrame@@', '@CFrameData@@')
    rem = [s for s in U0 - U1 if not hz(s)]; add = [s for s in U1 - U0 if not hz(s)]
    remp = set(pair(s) for s in rem)
    radd = [s for s in add if pair(s) not in remp and s != CTOR and s != IGF]
    rrem = [s for s in rem if not any(pair(x) == s for x in add) and 'InternalGetFrameForcedLoading' not in s and s != '??0CFrameData@@QAE@PAVCGraphicInfoFrame@@@Z']
    if rrem or radd: return 'diff -%s +%s' % (rrem[:3], radd[:3])
    return None
F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
