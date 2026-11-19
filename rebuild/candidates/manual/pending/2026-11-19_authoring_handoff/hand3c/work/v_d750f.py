A='        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)\n            dest[c_bone].AlignedPreScaleTransposed'
B='        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)\n            dest[c_bone].AlignedPreScale('
def mk(which, lit):
    def f(s):
        for x in which:
            assert x in s
            s=s.replace(x, x.replace('        for (', '        if (%s) {} else for (' % lit, 1))
        return s
    return f
VARIANTS=[('both0', mk([A,B],'0')),('bothF', mk([A,B],'false')),('T0', mk([A],'0')),('N0', mk([B],'0'))]
