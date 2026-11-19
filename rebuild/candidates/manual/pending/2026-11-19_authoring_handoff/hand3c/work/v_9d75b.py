A='''        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScale(bone_morphs[c_bone]);'''
T='''        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScaleTransposed(bone_morphs[c_bone]);'''
VARIANTS=[
 ('ptrloop', lambda s: s.replace(A,'''        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone, ++dest, ++bone_morphs)
            dest->AlignedPreScale(*bone_morphs);''')),
 ('postinc', lambda s: s.replace(A,A.replace('++c_bone','c_bone++')).replace(T,T.replace('++c_bone','c_bone++'))),
 ('elsefirst', lambda s: s.replace('    if (transpose) {\n'+T+'\n    } else {\n'+A+'\n    }','    if (!transpose) {\n'+A+'\n    } else {\n'+T+'\n    }')),
]
