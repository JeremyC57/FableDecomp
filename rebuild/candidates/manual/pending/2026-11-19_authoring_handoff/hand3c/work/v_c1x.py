M='    if (z < 0.0f)\n        z *= -1.0f;\n'
def rep(b):
    def f(s):
        assert M in s
        return s.replace(M,b)
    return f
VARIANTS=[('cond', rep('    z = z < 0.0f ? z * -1.0f : z;\n')),('scale', rep('    float mirror = 1.0f;\n    if (z < 0.0f)\n        z *= -1.0f;\n'))]
