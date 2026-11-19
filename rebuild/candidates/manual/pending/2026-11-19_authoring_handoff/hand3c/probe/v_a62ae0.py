import re
HEAD,REST=None,None
def body(lines, loop=None, cond='c1 < count'):
    def f(s):
        k=s.index('{\n    const unsigned char*'); e=s.rindex('}')
        lp=loop or ('    for (unsigned long c1 = 0; %s; ++c1) {\n        ReadNBytes(src_val, src_ptr, src_bytes);\n        src_val &= input_bit_mask;\n        if (min_value > src_val)\n            min_value = src_val;\n        src_ptr += src_stride;\n    }\n' % cond)
        return s[:k]+'{\n'+''.join('    '+l+'\n' for l in lines)+lp+'    return min_value;\n'+s[e:]
    return f
P='const unsigned char* src_ptr = (const unsigned char*)src_array;'
M='unsigned long min_value = count ? 0xffffffff : 0;'
V='unsigned long src_val = 0;'
B='unsigned long src_bytes = (src_bits + 7) >> 3;'
B8='unsigned long src_bytes = (src_bits + 7) / 8;'
K='unsigned long input_bit_mask = BitMasks[src_bits];'
VARIANTS=[
 ('egod_order', body([P,M,V,B,K])),
 ('egod_order_div', body([P,M,V,B8,K])),
 ('P_K_B_M_V', body([P,K,B,M,V])),
 ('P_B_K_M_V', body([P,B,K,M,V])),
 ('P_M_V_K_B', body([P,M,V,K,B])),
 ('egod_while', body([P,M,V,B,K], loop='    for (unsigned long c1 = count; c1 != 0; --c1) {\n        ReadNBytes(src_val, src_ptr, src_bytes);\n        src_val &= input_bit_mask;\n        if (min_value > src_val)\n            min_value = src_val;\n        src_ptr += src_stride;\n    }\n')),
]
import itertools
D=dict(P=P,M=M,V=V,B=B,K=K)
VARIANTS=[]
for perm in itertools.permutations('MVBK'):
    VARIANTS.append(('P'+''.join(perm), body([D['P']]+[D[c] for c in perm])))
