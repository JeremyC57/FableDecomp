O='    if (z < 0.0f)\n        z *= -1.0f;\n'
def r(new):
    def f(s):
        assert O in s
        return s.replace(O,new)
    return f
VARIANTS=[('negfirst', r('    if (z < 0.0f)\n        z = -1.0f * z;\n')),('zmul', r('    if (z < 0.0f)\n        z = z * -1.0f;\n')),
          ('gt', r('    if (0.0f > z)\n        z *= -1.0f;\n')),('negfirst_gt', r('    if (0.0f > z)\n        z = -1.0f * z;\n'))]
