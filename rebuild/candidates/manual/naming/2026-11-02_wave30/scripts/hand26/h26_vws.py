# h26_vws.py: naming wave 26, family vws -- the vector<CWideString>::_M_insert_overflow 0x00412330 stand-in callers (c691 queue row; scope 2):
#  each row's CWideStringVector becomes STLport 4.6 std::vector<CWideString, allocator<CWideString> > with the protected
#  _M_insert_overflow(..., const __false_type&, size_type I, bool) (vecify.py).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from vecify import vecify
NOTE = ('// Naming wave 26 (family vws): the vector is STLport 4.6 std::vector<CWideString, allocator<CWideString> >; its grow-insert 0x00412330\n'
        '//   is the protected ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAEXPAVCWideString@@ABV3@ABU__false_type@2@I_N@Z\n'
        '//   (size_type unsigned int; c691 queue). Was CWideStringVector::_M_insert_overflow(..., const __false_type&, %s, bool).')
for a, sz in (('009aef60', 'unsigned int'), ('009af480', 'unsigned int'), ('005fdad0', 'unsigned long'), ('0071ce80', 'unsigned long')):
    vecify(a, 'CWideStringVector', '_M_insert_overflow', '__false_type', elem='CWideString', note=NOTE % sz, stdtag='__false_type')
