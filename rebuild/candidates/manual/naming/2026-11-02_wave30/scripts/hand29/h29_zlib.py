# h29_zlib.py: naming wave 29, family zlib (scope 3, c698 queue): retail's zlib is built with Z_PREFIX -- both PDBs spell _z_inflateEnd
#  (0x00C038D0) and _z_deflateEnd (0x00C00AE0), which six landed rows already bind. 0x00A3ABB0 (FinishDecompressStream) called it as
#  _inflateEnd on an invented CCompressorZlib_Retail class; 0x00A3A930 (FinishCompressStream) as _EndDeflateStream. Both own rows now name
#  their Ego_r.pdb / FableWin.pdb members ?FinishDecompressStream@CCompressorZlib@@QAEXXZ / ?FinishCompressStream@CCompressorZlib@@QAEXXZ
#  in a Leaf line (both were Leaf-less). The z_stream is zlib's struct z_stream_s.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
a = '00a3abb0'
he.edit_all(a, 'src', 'z_stream_s_Local', 'z_stream_s')
he.edit(a, 'src', 'extern "C" int __cdecl inflateEnd(z_stream_s* stream);   // z_inflateEnd @ 0x00C038D0',
        'extern "C" int __cdecl z_inflateEnd(z_stream_s* stream); // 0x00C038D0 (zlib built with Z_PREFIX: both PDBs spell _z_inflateEnd)')
he.edit(a, 'src', '        inflateEnd(stream);', '        z_inflateEnd(stream);')
he.edit_all(a, 'src', 'CCompressorZlib_Retail', 'CCompressorZlib')
note(a, 'zlib', 'the own row is ?FinishDecompressStream@CCompressorZlib@@QAEXXZ (Ego_r.pdb / FableWin.pdb; was the invented\n'
     '//   CCompressorZlib_Retail); it calls _z_inflateEnd 0x00C038D0 (both PDBs; was _inflateEnd). Local layout kept (header debt: PDB +4).',
     leaf='?FinishDecompressStream@CCompressorZlib@@QAEXXZ')
a = '00a3a930'
he.edit(a, 'src', 'extern "C" int __cdecl EndDeflateStream(void* strm);',
        'struct z_stream_s;\nextern "C" int __cdecl z_deflateEnd(z_stream_s* strm);   // 0x00C00AE0 (zlib built with Z_PREFIX: both PDBs spell _z_deflateEnd)')
he.edit(a, 'src', '        EndDeflateStream(&this->zstream);', '        z_deflateEnd((z_stream_s*)&this->zstream);')
note(a, 'zlib', 'the stream is closed by _z_deflateEnd 0x00C00AE0 (both PDBs; was the invented _EndDeflateStream); own Leaf\n'
     '//   ?FinishCompressStream@CCompressorZlib@@QAEXXZ (Ego_r.pdb / FableWin.pdb).', leaf='?FinishCompressStream@CCompressorZlib@@QAEXXZ')
he.edit_all(a, 'tst', 'EndDeflateStream', 'z_deflateEnd', 2)   # the test links the row: its stub carries the bound name
